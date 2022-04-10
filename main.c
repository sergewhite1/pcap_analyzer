#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#pragma pack (push, 1)

typedef struct __packet_record_t
{
  uint32_t timestamp_seconds;
  uint32_t timestamp_ms_or_ns;
  uint32_t captured_packet_length;
  uint32_t original_packet_length;
} packet_record_t;

// IEEE 802.1Q with TCI (Tag Control Information)
typedef struct __ethernet_hdr_t
{
  uint8_t  dst_mac_address[6];
  uint8_t  src_mac_address[6];
  uint16_t ether_type;  // must be 0x8100 in host byte order
  uint16_t tci;
  uint16_t ether_type2; // must be 0x8000 in host byte order - IPv4
} ethernet_hdr_t;

typedef struct __ip_packet_t
{
  uint8_t  version_header_length;
  uint8_t  type_of_service;
  uint16_t total_length;
  uint16_t indentification;
  uint16_t flags_fragment_offset;
  uint8_t  time_to_leave;
  uint8_t  protocol;
  uint16_t header_checksum;
  uint32_t src_address;
  uint32_t dst_address;
} ip_packet_t;

typedef struct __udp_hdr_t
{
  uint16_t src_port;
  uint16_t dst_port;
  uint16_t total_length;
  uint16_t checksum;
} udp_hdr_t;

#pragma pach (pop)

char* mac_addr_to_str(const uint8_t* mac_addr_ptr)
{
  static char ret [128];
  sprintf(ret,
         "%02X:%02X:%02X:%02X:%02X:%02X",
         mac_addr_ptr[0],
         mac_addr_ptr[1],
         mac_addr_ptr[2],
         mac_addr_ptr[3],
         mac_addr_ptr[4],
         mac_addr_ptr[5]);
  return ret;
}

uint8_t get_ip_hdr_version(const ip_packet_t* ip_packet_ptr)
{
  return ip_packet_ptr->version_header_length >> 4;
}

uint16_t get_ip_hdr_len_in_bytes(const ip_packet_t* ip_packet_ptr)
{
  return (ip_packet_ptr->version_header_length & 0xF) * 4; 
}

int main(int argc, char* argv[])
{
  printf("Hello!\n");
  //for (int i = 0; i < argc; ++i)
  //{
  //  printf("argv[%d]=%s\n", i, argv[i]);
  //}

  if (argc < 2)
  {
    printf("Error! Filename is missed!\n");
    return 1;
  }

  const char* src_filename = argv[1];
  const char* dst_filename = "result.pcap";
  int ret = 0;
  FILE* src_file = NULL;
  FILE* dst_file = NULL;

  const int BUFF_SIZE = 2048;

  char buff[BUFF_SIZE];

  packet_record_t* packet_record_ptr = (packet_record_t*)buff;
  ethernet_hdr_t* ethernet_hdr_ptr = (ethernet_hdr_t*)(buff + sizeof(packet_record_t));
  ip_packet_t* ip_packet_ptr = (ip_packet_t*)(buff + sizeof(packet_record_t) + sizeof(ethernet_hdr_t));

  int packet_index = 0;

  do
  {
    src_file = fopen(src_filename, "r");
    int errnum = errno;
    if (src_file == NULL)
    {
      printf("Error open file: %s\n", src_filename);
      printf("%s\n", strerror(errnum));
      ret = 1;
      break;
    }

    dst_file = fopen(dst_filename, "w");
    if (dst_file == NULL)
    {
      printf("Error open file for write: %s\n", dst_filename);
      ret = 1;
      break;
    }

    size_t read_count = fread(buff, 1, 24, src_file);
    if (read_count != 24)
    {
      printf("Error of reading 24 bytes (pcap header).\n");
      ret = 1;
      break;
    }

    fwrite(buff, 1, 24, dst_file);

    // Write Packet Records to dst file
    while(1)
    {
      if (packet_index % 1000000 == 0)
      {
        printf("packet_index=%d\n", packet_index);
      }
      read_count = fread(buff, 1, sizeof(packet_record_t), src_file);
      if (read_count != sizeof(packet_record_t))
      {
        printf("current packet_index=%d\n", packet_index);
        break;
      }

      if (packet_record_ptr->captured_packet_length !=
          packet_record_ptr->original_packet_length)
      {
        printf("captured_packet_length=%d\n", packet_record_ptr->captured_packet_length);
        printf("original_packet_length=%d\n", packet_record_ptr->original_packet_length);
        ret = 1;
        break;
      }

      read_count = fread(buff + sizeof(packet_record_t),
                         1,
                         packet_record_ptr->captured_packet_length,
                         src_file);

      if (read_count != packet_record_ptr->captured_packet_length)
      {
        printf("Error reading captured packet data.\n");
        ret = 1;
        break;
      }

      //printf("dst MAC address: %s\n", mac_addr_to_str(ethernet_hdr_ptr->dst_mac_address));
      //printf("src MAC address: %s\n", mac_addr_to_str(ethernet_hdr_ptr->src_mac_address));
      //printf("ether_type : 0x%02X\n", ethernet_hdr_ptr->ether_type);
      //printf("ether_type2: 0x%02X\n", ethernet_hdr_ptr->ether_type2);

      if (ntohs(ethernet_hdr_ptr->ether_type) == 0x8100)
      {
        ip_packet_ptr = (ip_packet_t*)(buff + sizeof(packet_record_t) + sizeof(ethernet_hdr_t));
        if (ntohs(ethernet_hdr_ptr->ether_type2) != 0x0800)
        {
          printf("Error! ethernet_hdr_ptr->ether_type2=0x%02X Expected value: 0x0800 (Type: IPv4)\n",
                ntohs(ethernet_hdr_ptr->ether_type2));
          ret = 1;
          break;
        }
      }
      else if (ntohs(ethernet_hdr_ptr->ether_type) == 0x800)
      {
        ip_packet_ptr = (ip_packet_t*)(buff + sizeof(packet_record_t) + sizeof(ethernet_hdr_t) - 4);
      }
      else
      {
        printf("Error! ethernet_hdr_ptr->ether_type=0x%02X Expected values: 0x800 (Type: IPv4), 0x8100 (Type: 802.1Q Virtual LAN)\n",
            ntohs(ethernet_hdr_ptr->ether_type));
        ret = 1;
        break;
      }


      uint8_t version = get_ip_hdr_version(ip_packet_ptr);
      if (version != 4)
      {
        printf("Error! Version in IP header=%d Expected value: 4\n", version);
        ret = 1;
        break;
      }

      if (ip_packet_ptr->protocol != 17)
      {
        printf("Error! ip_packet_ptr->protocol=%d Expected value: 17 (UDP)\n", ip_packet_ptr->protocol);
        ret = 1;
        break;
      }

      //printf("version: %d\n", version);

      uint16_t ip_packet_hdr_len = get_ip_hdr_len_in_bytes(ip_packet_ptr);
      printf("ip_packet_hdr_len=%d bytes \n", ip_packet_hdr_len);
      printf("ip_packet_ptr->total_length=%d\n", ntohs(ip_packet_ptr->total_length));
      printf("ip_packet_ptr->protocol=%d\n", ip_packet_ptr->protocol);

      udp_hdr_t * udp_hdr_ptr = (udp_hdr_t*)((char*)(ip_packet_ptr) + get_ip_hdr_len_in_bytes(ip_packet_ptr));
      printf("udp_hdr_ptr->src_port=%d\n", ntohs(udp_hdr_ptr->src_port));
      printf("udp_hdr_ptr->total_length=%d\n", ntohs(udp_hdr_ptr->total_length));

      uint8_t * udp_data = (uint8_t*)udp_hdr_ptr + sizeof(udp_hdr_t);
      uint32_t  udp_data_len = ntohs(udp_hdr_ptr->total_length) - sizeof(udp_hdr_t);

      printf("udp_data_len=%d\n", udp_data_len);

      printf("\n");

      int need_copy = 0;

      // ...
      
      if (need_copy)
      {
        fwrite(buff,
               1,
               sizeof(packet_record_t) + packet_record_ptr->captured_packet_length,
               dst_file);
      }

      ++packet_index;
    }
  } while (0);

  if (dst_file != NULL)
  {
    fclose(dst_file);
  }

  if (src_file != NULL)
  {
    fclose(src_file);
  }

  return ret;
}
