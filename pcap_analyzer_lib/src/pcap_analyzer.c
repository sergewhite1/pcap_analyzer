#include "../include/pcap_analyzer.h"

#include <errno.h>
#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

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

uint16_t get_tcp_hdr_len_in_bytes(const tcp_hdr_t* tcp_hdr_ptr)
{
  return (*((uint8_t*)&tcp_hdr_ptr->codes) >> 4) * 4;
}
static int verbose = 1;

int process_pcap_file(const char* inp_filename,
                      const char* out_filename,
                      packet_process_func_t callback)
{
  int ret = 0;
  FILE* inp_file = NULL;
  FILE* out_file = NULL;

  const uint32_t BUFF_SIZE = 2048;
  const uint32_t MAX_PACKET_BODY_SIZE = BUFF_SIZE - sizeof(pcap_file_hdr_t);

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

    if (verbose)
    {
      printf("pcap version: %d.%d\n",
             pcap_file_hdr_ptr->major_version,
             pcap_file_hdr_ptr->minor_version);
    }

    if (pcap_file_hdr_ptr->magic_number ==
        PCAP_FILE_HDR_MAGIC_NUMBER_MICROSECONDS)
    {
      if (verbose)
      {
        printf("pcap packet timestamp: microseconds\n");
      }
    }
    else if (pcap_file_hdr_ptr->magic_number == PCAP_FILE_HDR_MAGIC_NUMBER_NANOSECONDS)
    {
      if (verbose)
      {
        printf("pcap packet timestamp: nanoseconds\n");
      }
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

    if (verbose)
    {
      printf("snap len: %d bytes\n", pcap_file_hdr_ptr->snap_len);
    }

    if (out_file)
    {
      fwrite(buff, 1, sizeof(pcap_file_hdr_t), out_file);
    }

    // Analyze Pcap Packets
    while(1)
    {
      ++packet_index;

      memset(&packet_desc, 0, sizeof(packet_desc));
      packet_desc.packet_record_ptr = (packet_record_t*)buff;

      if (verbose && packet_index % 1000000 == 0)
      {
        printf("packet_index=%d\n", packet_index);
      }

      read_count = fread(buff, 1, sizeof(packet_record_t), inp_file);

      if (read_count != sizeof(packet_record_t))
      {
        if (verbose)
        {
          printf("current packet_index=%d\n", packet_index);
        }
        break;
      }

      packet_desc.index = packet_index;

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

      if (packet_desc.packet_record_ptr->captured_packet_length >=
          MAX_PACKET_BODY_SIZE)
      {
        printf("packet_desc.packet_record_ptr->captured_packet_length=%d"
               " but MAX_PACKET_BODY_SIZE=%d\n",
               packet_desc.packet_record_ptr->captured_packet_length,
               MAX_PACKET_BODY_SIZE);
        ret = 1;
        break;
      }

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

      uint8_t version = get_ip_hdr_version(packet_desc.ip_packet_ptr);
      if (version != 4)
      {
        printf("Error! Version in IP header=%d Expected value: 4\n", version);
        ret = 1;
        break;
      }

      void* transport_hdr_ptr = (void*)(packet_desc.ip_packet_ptr) +
            get_ip_hdr_len_in_bytes(packet_desc.ip_packet_ptr);

      switch(packet_desc.ip_packet_ptr->protocol)
      {
        case IPPROTO_TCP:
          packet_desc.tcp_hdr_ptr = (tcp_hdr_t*)transport_hdr_ptr;

          packet_desc.data = (uint8_t*)packet_desc.tcp_hdr_ptr +
            get_tcp_hdr_len_in_bytes(packet_desc.tcp_hdr_ptr);

          packet_desc.data_size =
            htons(packet_desc.ip_packet_ptr->total_length)     -
            get_ip_hdr_len_in_bytes (packet_desc.ip_packet_ptr) -
            get_tcp_hdr_len_in_bytes(packet_desc.tcp_hdr_ptr);

        break;

        case IPPROTO_UDP:
          packet_desc.udp_hdr_ptr = (udp_hdr_t*)transport_hdr_ptr;

           packet_desc.data = (uint8_t*) packet_desc.udp_hdr_ptr +
                                         sizeof(udp_hdr_t);
           packet_desc.data_size =
             ntohs(packet_desc.udp_hdr_ptr->total_length) - sizeof(udp_hdr_t);

        break;

        default:
          printf("Error! ip_packet_ptr->protocol=%d"
                 " Expected values: %d (TCP), %d (UDP)\n",
                  packet_desc.ip_packet_ptr->protocol,
                  IPPROTO_TCP,
                  IPPROTO_UDP);
           ret = 1;
      }

      if (ret)
      {
        break;
      }

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

void pcap_analyzer_set_verbose(int val)
{
  verbose = val;
}
