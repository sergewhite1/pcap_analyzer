#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#define PCAP_FILE_HDR_MAGIC_NUMBER_MICROSECONDS 0xA1B2C3D4
#define PCAP_FILE_HDR_MAGIC_NUMBER_NANOSECONDS  0xA1B23C4D

#pragma pack (push, 1)

// https://tools.ietf.org/id/draft-gharris-opsawg-pcap-00.html

/*
                           1                   2                   3
       0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    0 |                          Magic Number                         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    4 |          Major Version        |         Minor Version         |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    8 |                           Reserved1                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   12 |                           Reserved2                           |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   16 |                            SnapLen                            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   20 | FCS |f|                   LinkType                            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    Figure 1: File Header
*/


typedef struct __pcap_file_hdr_t
{
  uint32_t magic_number;
  uint16_t major_version;
  uint16_t minor_version;
  uint32_t reserved1;
  uint32_t reserved2;
  uint32_t snap_len;
  uint32_t link_type;

} pcap_file_hdr_t;

/*
                        1                   2                   3
      0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    0 |                      Timestamp (Seconds)                      |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    4 |            Timestamp (Microseconds or nanoseconds)            |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    8 |                    Captured Packet Length                     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   12 |                    Original Packet Length                     |
      +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
   16 /                                                               /
      /                          Packet Data                          /
      /                        variable length                        /
      /                                                               /
      +---------------------------------------------------------------+
*/
typedef struct __packet_record_t
{
  uint32_t timestamp_seconds;
  uint32_t timestamp_ms_or_ns;
  uint32_t captured_packet_length;
  uint32_t original_packet_length;
} packet_record_t;

#define ETH_PROTOCOL_IP4   0x0800
#define ETH_PROTOCOL_8021Q 0x8100

typedef struct __ethernet_hdr_t
{
  uint8_t  dst_mac_address[6];
  uint8_t  src_mac_address[6];
  uint16_t ether_type;

  // IEEE 802.1Q with TCI (Tag Control Information)
  uint16_t tci;
  uint16_t ether_type2;

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

typedef struct __packet_desc_t
{
  int              index;
  packet_record_t* packet_record_ptr;
  ethernet_hdr_t*  ethernet_hdr_ptr;
  ip_packet_t*     ip_packet_ptr;
  udp_hdr_t*       udp_hdr_ptr;
  uint8_t*         data;
} packet_desc_t;

typedef enum __action_t {
  action_skip, action_copy, action_copy_and_stop, action_stop} action_t;

typedef action_t (*packet_process_func_t)(const packet_desc_t*);

action_t packet_process_example(const packet_desc_t* packet_desc_ptr);

packet_process_func_t on_packet_process = packet_process_example;

int process_pcap_file(const char* inp_filename,
                      const char* out_filename,
                      packet_process_func_t callback)
{
  int ret = 0;
  FILE* inp_file = NULL;
  FILE* out_file = NULL;

  const int BUFF_SIZE = 2048;
  const int PACKET_BODY_SIZE = BUFF_SIZE - sizeof(pcap_file_hdr_t);

  char buff[BUFF_SIZE];

  packet_desc_t packet_desc;
  memset(&packet_desc, 0, sizeof(packet_desc));
  packet_desc.packet_record_ptr = (packet_record_t*)buff;

  int packet_index = 0;

  do
  {
    inp_file = fopen(inp_filename, "r");
    int errnum = errno;
    if (inp_file == NULL)
    {
      printf("Error open file: %s\n", inp_filename);
      printf("%s\n", strerror(errnum));
      ret = 1;
      break;
    }

    if (out_filename)
    {
      out_file = fopen(out_filename, "w");
      if (out_file == NULL)
      {
        printf("Error open file for write: %s\n", out_filename);
        ret = 1;
        break;
      }
    }

    size_t read_count = fread(buff, 1, sizeof(pcap_file_hdr_t), inp_file);
    if (read_count != sizeof(pcap_file_hdr_t))
    {
      printf("Error of reading %ld bytes (pcap header).\n",
             sizeof(pcap_file_hdr_t));
      ret = 1;
      break;
    }

    pcap_file_hdr_t* pcap_file_hdr_ptr = (pcap_file_hdr_t*)buff;

    printf("pcap version: %d.%d\n",
           pcap_file_hdr_ptr->major_version,
           pcap_file_hdr_ptr->minor_version);

    if (pcap_file_hdr_ptr->magic_number == PCAP_FILE_HDR_MAGIC_NUMBER_MICROSECONDS)
    {
      printf("pcap packet timestamp: microseconds\n");
    }
    else if (pcap_file_hdr_ptr->magic_number == PCAP_FILE_HDR_MAGIC_NUMBER_NANOSECONDS)
    {
      printf("pcap packet timestamp: nanoseconds\n");
    }
    else
    {
      printf("Unexpected pcap header magic number= 0x%X Expected values: 0x%X, 0x%X \n",
             pcap_file_hdr_ptr->magic_number,
             PCAP_FILE_HDR_MAGIC_NUMBER_MICROSECONDS,
             PCAP_FILE_HDR_MAGIC_NUMBER_NANOSECONDS);
      ret = 1;
      break;
    }

    printf("snap len: %d bytes\n", pcap_file_hdr_ptr->snap_len);

    if (out_file)
    {
      fwrite(buff, 1, sizeof(pcap_file_hdr_t), out_file);
    }

    // Analyze Pcap Packets
    while(1)
    {
      if (packet_index % 1000000 == 0)
      {
        printf("packet_index=%d\n", packet_index);
      }

      read_count = fread(buff, 1, sizeof(packet_record_t), inp_file);

      if (read_count != sizeof(packet_record_t))
      {
        printf("current packet_index=%d\n", packet_index);
        break;
      }

      ++packet_index;
      packet_desc.index = packet_index;

      printf("!!!timestamp seconds: %d\n", packet_desc.packet_record_ptr->timestamp_seconds);

      if (packet_desc.packet_record_ptr->captured_packet_length !=
          packet_desc.packet_record_ptr->original_packet_length)
      {
        printf("captured_packet_length=%d\n",
                packet_desc.packet_record_ptr->captured_packet_length);

        printf("original_packet_length=%d\n",
                packet_desc.packet_record_ptr->original_packet_length);
        ret = 1;
        break;
      }

      packet_desc.ethernet_hdr_ptr = (ethernet_hdr_t*)
      (buff + sizeof(packet_record_t));

      read_count = fread(packet_desc.ethernet_hdr_ptr,
                         1,
                         packet_desc.packet_record_ptr->captured_packet_length,
                         inp_file);

      if (read_count != packet_desc.packet_record_ptr->captured_packet_length)
      {
        printf("Error reading captured packet data.\n");
        ret = 1;
        break;
      }

//      printf("dst MAC address: %s\n",
//             mac_addr_to_str(packet_desc.ethernet_hdr_ptr->dst_mac_address));
//
//      printf("src MAC address: %s\n",
//             mac_addr_to_str(packet_desc.ethernet_hdr_ptr->src_mac_address));
//
//      printf("ether_type : 0x%02X\n",
//             ntohs(packet_desc.ethernet_hdr_ptr->ether_type));
//
//      printf("ether_type2: 0x%02X\n",
//             ntohs(packet_desc.ethernet_hdr_ptr->ether_type2));

      if (ntohs(packet_desc.ethernet_hdr_ptr->ether_type) == ETH_PROTOCOL_8021Q)
      {
        packet_desc.ip_packet_ptr = (ip_packet_t*)
        (buff + sizeof(packet_record_t) + sizeof(ethernet_hdr_t));

        if (ntohs(packet_desc.ethernet_hdr_ptr->ether_type2) != ETH_PROTOCOL_IP4)
        {
          printf("Error! ethernet_hdr_ptr->ether_type2=0x%X Expected value: 0x%X (Type: IPv4)\n",
                ntohs(packet_desc.ethernet_hdr_ptr->ether_type2),
                ETH_PROTOCOL_IP4);
          ret = 1;
          break;
        }
      }
      else if (ntohs(packet_desc.ethernet_hdr_ptr->ether_type) == ETH_PROTOCOL_IP4)
      {
        packet_desc.ip_packet_ptr = (ip_packet_t*)
        (buff + sizeof(packet_record_t) + sizeof(ethernet_hdr_t) - 4);
      }
      else
      {
        printf("Error! ethernet_hdr_ptr->ether_type=0x%X"
               " Expected values: 0x%X (Type: IPv4),"
               " 0x%X (Type: 802.1Q Virtual LAN)\n",
               ntohs(packet_desc.ethernet_hdr_ptr->ether_type),
               ETH_PROTOCOL_IP4,
               ETH_PROTOCOL_8021Q);
        ret = 1;
        break;
      }

//      uint8_t version = get_ip_hdr_version(ip_packet_ptr);
//      if (version != 4)
//      {
//        printf("Error! Version in IP header=%d Expected value: 4\n", version);
//        ret = 1;
//        break;
//      }

//      if (ip_packet_ptr->protocol != 17)
//      {
//        printf("Error! ip_packet_ptr->protocol=%d Expected value: 17 (UDP)\n", ip_packet_ptr->protocol);
//        ret = 1;
//        break;
//      }

      //printf("version: %d\n", version);

//      uint16_t ip_packet_hdr_len = get_ip_hdr_len_in_bytes(ip_packet_ptr);
//      printf("ip_packet_hdr_len=%d bytes \n", ip_packet_hdr_len);
//      printf("ip_packet_ptr->total_length=%d\n", ntohs(ip_packet_ptr->total_length));
//      printf("ip_packet_ptr->protocol=%d\n", ip_packet_ptr->protocol);
//
//      udp_hdr_t * udp_hdr_ptr = (udp_hdr_t*)((char*)(ip_packet_ptr) + get_ip_hdr_len_in_bytes(ip_packet_ptr));
//      printf("udp_hdr_ptr->src_port=%d\n", ntohs(udp_hdr_ptr->src_port));
//      printf("udp_hdr_ptr->total_length=%d\n", ntohs(udp_hdr_ptr->total_length));

//      uint8_t * udp_data = (uint8_t*)udp_hdr_ptr + sizeof(udp_hdr_t);
//      uint32_t  udp_data_len = ntohs(udp_hdr_ptr->total_length) - sizeof(udp_hdr_t);

//      printf("udp_data_len=%d\n", udp_data_len);

//      printf("\n");

//      int need_copy = 0;

      action_t action = callback(&packet_desc);

      if (out_file &&
            (action == action_copy ||
             action == action_copy_and_stop))
      {
        fwrite(buff,
               1,
               sizeof(packet_record_t) + packet_desc.packet_record_ptr->captured_packet_length,
               out_file);

        if (action == action_copy_and_stop)
        {
          break;
        }
      }
      else if (action == action_stop)
      {
        break;
      }
    }
  } while (0);

  if (out_file != NULL)
  {
    fclose(out_file);
  }

  if (inp_file != NULL)
  {
    fclose(inp_file);
  }

  return ret;
}

int ipv4_udp_callback_result;

action_t ipv4_udp_test_callback(const packet_desc_t* packet_desc_ptr)
{
  printf("ipv4_udp_test_callback\n");
  ipv4_udp_callback_result = 1;

  const int packet_index_expected = 1;

  if (packet_desc_ptr->index != packet_index_expected)
  {
    printf("packet_desc_ptr->index=%d expected value=%d\n",
           packet_desc_ptr->index,
           packet_index_expected);
    return action_skip;
  }

  const packet_record_t* packet_record_ptr =
  packet_desc_ptr->packet_record_ptr;

  const uint32_t packet_timestamp_seconds_expected = 1649599520;
  if (packet_record_ptr->timestamp_seconds !=
      packet_timestamp_seconds_expected)
  {
    printf("packet_record_ptr->timestamp_seconds"
           " actual value: %d expected value: %d\n",
           packet_record_ptr->timestamp_seconds,
           packet_timestamp_seconds_expected);
    return action_skip;
  }

  uint32_t packet_timestamp_ms_expected = 924595;
  if (packet_record_ptr->timestamp_ms_or_ns !=
      packet_timestamp_ms_expected)
  {
    printf("packet_record_ptr->timestamp_ms_or_ns"
           " actual value: %d expected value: %d\n",
           packet_record_ptr->timestamp_ms_or_ns,
           packet_timestamp_ms_expected);
    return action_skip;
  }

  uint32_t captured_packet_length_expected = 80;
  if (packet_record_ptr->captured_packet_length !=
      captured_packet_length_expected)
  {
    printf("packet_record_ptr->captured_packet_length"
           " actual value: %d expected value: %d\n",
           packet_record_ptr->captured_packet_length,
           captured_packet_length_expected);
    return action_skip;
  }

  uint32_t original_packet_length_expected = 80;
  if (packet_record_ptr->original_packet_length !=
      original_packet_length_expected)
  {
    printf("packet_record_ptr->original_packet_length"
           " actual result: %d expected result: %d\n",
           packet_record_ptr->original_packet_length,
           original_packet_length_expected);
    return action_skip;
  }

  ethernet_hdr_t* ethernet_hdr_ptr = packet_desc_ptr->ethernet_hdr_ptr;

  const char* dst_mac_address_actual =
  mac_addr_to_str(ethernet_hdr_ptr->dst_mac_address);

  const char* dst_mac_address_expected = "01:00:5E:7F:FF:FB";

  if (strcmp(dst_mac_address_actual, dst_mac_address_expected) != 0)
  {
    printf("dst_mac_address actual result: %s expected result: %s\n",
           dst_mac_address_actual,
           dst_mac_address_expected);
    return action_skip;
  }

  const char* src_mac_address_actual =
  mac_addr_to_str(ethernet_hdr_ptr->src_mac_address);

  const char* src_mac_address_expected = "8C:5A:F8:EB:6F:01";

  if (strcmp(src_mac_address_actual, src_mac_address_expected) != 0)
  {
    printf("src_mac_address actual result: %s expected result: %s\n",
           src_mac_address_actual,
           src_mac_address_expected);

    return action_skip;
  }

  if (ntohs(ethernet_hdr_ptr->ether_type) != ETH_PROTOCOL_IP4)
  {
    printf("ntohs(ethernet_hdr_ptr->ether_type) actual result: 0x%X"
           " expected result: 0x%X (Type IPv4)",
           ntohs(ethernet_hdr_ptr->ether_type),
           ETH_PROTOCOL_IP4);

    return action_skip;
  }

  //TODO: Continue testing

  ipv4_udp_callback_result = 0;
  return action_skip;
}

int ipv4_udp_test()
{
  process_pcap_file("./pcap/ipv4_udp.pcap",
                    NULL,
                    ipv4_udp_test_callback);
  return ipv4_udp_callback_result;
}

int run_tests()
{
  printf("run tests...\n");

  int ret = 0;

  // UDP IPv4
  if (ipv4_udp_test() == 0)
  {
    printf("ipv4_udp_test: SUCCESS\n");
  }
  else
  {
    ret = 1;
    printf("udp_ipv4_test: FAILED\n");
  }



  return ret;
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

  if (strcmp(argv[1], "--run-tests") == 0)
  {
    return run_tests();
  }

  const char* src_filename = argv[1];
  const char* dst_filename = "result.pcap";
  int ret = 0;
  FILE* src_file = NULL;
  FILE* dst_file = NULL;

  const int BUFF_SIZE = 2048;

  char buff[BUFF_SIZE];

  packet_desc_t packet_desc;
  memset(&packet_desc, 0, sizeof(packet_desc));

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

    // Analyze Packet Records
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

      action_t action = on_packet_process(&packet_desc);

      if (action == action_copy ||
          action == action_copy_and_stop)
      {
        fwrite(buff,
               1,
               sizeof(packet_record_t) + packet_record_ptr->captured_packet_length,
               dst_file);

        if (action == action_copy_and_stop)
        {
          break;
        }
      }
      else if (action == action_stop)
      {
        break;
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

action_t packet_process_example(const packet_desc_t* packet_desc_ptr)
{

}