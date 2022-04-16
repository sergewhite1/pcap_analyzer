#ifndef PCAP_ANALYZER_H
#define PCAP_ANALYZER_H

#include <stdint.h>

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

char* mac_addr_to_str(const uint8_t* mac_addr_ptr);
uint8_t get_ip_hdr_version(const ip_packet_t* ip_packet_ptr);
uint16_t get_ip_hdr_len_in_bytes(const ip_packet_t* ip_packet_ptr);

typedef struct __packet_desc_t
{
  int              index;
  packet_record_t* packet_record_ptr;
  ethernet_hdr_t*  ethernet_hdr_ptr;
  ip_packet_t*     ip_packet_ptr;
  udp_hdr_t*       udp_hdr_ptr;
  uint8_t*         data;
  uint16_t         data_size;
} packet_desc_t;

typedef enum __action_t {
  action_skip, action_copy, action_copy_and_stop, action_stop} action_t;

typedef action_t (*packet_process_func_t)(const packet_desc_t*);

int process_pcap_file(const char* inp_filename,
                      const char* out_filename,
                      packet_process_func_t callback);

int pcap_analyzer_set_verbose(int val);

#endif // ! PCAP_ANALYZER_H
