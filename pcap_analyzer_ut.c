#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>

#include "pcap_analyzer.h"

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

  if (packet_record_ptr == NULL)
  {
    printf("packet_record_ptr == NULL\n");
    return action_skip;
  }

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

  if (ethernet_hdr_ptr == NULL)
  {
    printf("ethernet_hdr_ptr == NULL\n");
    return action_skip;
  }

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

  ip_packet_t* ip_packet_ptr = packet_desc_ptr->ip_packet_ptr;

  if (ip_packet_ptr == NULL)
  {
    printf("ip_packet_ptr == NULL\n");
    return action_skip;
  }

  uint8_t ip_version_expected = 4;
  uint8_t ip_version_actual = get_ip_hdr_version(ip_packet_ptr);

  if (ip_version_actual != ip_version_expected)
  {
    printf("ip_packet_ptr version"
           " actual result: %d expected result: %d\n",
           ip_version_actual,
           ip_version_expected);

    return action_skip;
  }

  uint16_t ip_hdr_len_expected = 20; // bytes
  uint16_t ip_hdr_len_actual = get_ip_hdr_len_in_bytes(ip_packet_ptr);

  if (ip_hdr_len_actual != ip_hdr_len_expected)
  {
   printf("ip_packet_ptr hdr_len_in_bytes"
          " actual result: %d expected result: %d\n",
          ip_hdr_len_actual,
          ip_hdr_len_expected);

    return action_skip;
  }

  if (ip_packet_ptr->type_of_service != 0)
  {
    printf("ip_packet_ptr->type_of_service"
           " actual result: %d expected result: %d\n",
           ip_packet_ptr->type_of_service,
           0);

    return action_skip;
  }

  uint16_t ip_packet_total_length_expected = 66;

  uint16_t ip_packet_total_length_actual =
  htons(ip_packet_ptr->total_length);

  if (ip_packet_total_length_actual != ip_packet_total_length_expected)
  {
    printf("ip packet total length"
           " actual result: %d expected result: %d\n",
           ip_packet_total_length_actual,
           ip_packet_total_length_expected);

    return action_skip;
  }

  uint16_t ip_packet_indentifiction_expected = 0xA932;
  uint16_t ip_packet_indentifiction_actual =
  htons(ip_packet_ptr->indentification);

  if (ip_packet_indentifiction_actual != ip_packet_indentifiction_expected)
  {
    printf("ip packet indentification"
           " actual result: %d expected result: %d\n",
           ip_packet_indentifiction_actual,
           ip_packet_indentifiction_expected);

    return action_skip;
  }

  uint16_t ip_packet_flags_fragment_offset_expected = 0x4000;
  uint16_t ip_packet_flags_fragment_offset_actual =
  htons(ip_packet_ptr->flags_fragment_offset);

  if (ip_packet_flags_fragment_offset_actual !=
      ip_packet_flags_fragment_offset_expected)
  {
    printf("ip packet flags fragment offset"
           " actual result: %d expected result: %d\n",
           ip_packet_flags_fragment_offset_actual,
           ip_hdr_len_expected);

    return action_skip;
  }

  if (ip_packet_ptr->time_to_leave != 1)
  {
    printf("ip_packet_ptr->time_to_leave"
           " actual result: %d expected result: %d\n",
           ip_packet_ptr->time_to_leave,
           1);

    return action_skip;
  }

  if (ip_packet_ptr->protocol != IPPROTO_UDP)
  {
    printf("ip_packet_ptr->protocol"
           " actual result: %d expected result: %d (UDP)",
           ip_packet_ptr->protocol,
           IPPROTO_UDP);

    return action_skip;
  }

  uint16_t ip_packet_hdr_checksum_expected = 0x43;
  uint16_t ip_packet_hdr_checksum_actual =
  ntohs(ip_packet_ptr->header_checksum);

  if (ip_packet_hdr_checksum_actual != ip_packet_hdr_checksum_expected)
  {
    printf("ip packet hdr checksum"
           " actual result: 0x%X expected result: 0x%X\n",
           ip_packet_hdr_checksum_actual,
           ip_packet_hdr_checksum_expected);

    return action_skip;
  }

  const char* src_addr_expected = "192.168.31.146";
  char src_addr_actual[INET_ADDRSTRLEN];
  struct sockaddr_in sa;
  sa.sin_addr.s_addr = ip_packet_ptr->src_address;
  inet_ntop(AF_INET, &(sa.sin_addr), src_addr_actual, INET_ADDRSTRLEN);

  if (strcmp(src_addr_actual, src_addr_expected) != 0)
  {
    printf("src addr actual result: %s expected result: %s\n",
          src_addr_actual,
          src_addr_expected);

    return action_skip;
  }

  const char* dst_addr_expected = "239.255.255.251";
  char dst_addr_actual[INET_ADDRSTRLEN];
  sa.sin_addr.s_addr = ip_packet_ptr->dst_address;
  inet_ntop(AF_INET, &(sa.sin_addr), dst_addr_actual, INET_ADDRSTRLEN);

  if (strcmp(dst_addr_actual, dst_addr_expected) != 0)
  {
    printf("dst addr actual result: %s expected result: %s\n",
           dst_addr_actual,
           dst_addr_expected);

    return action_skip;
  }

  udp_hdr_t* udp_hdr_ptr = packet_desc_ptr->udp_hdr_ptr;

  if (udp_hdr_ptr == NULL)
  {
    printf("udp_hdr_ptr == NULL\n");
    return action_skip;
  }

  uint16_t src_port_expected = 10101;
  uint16_t src_port_actual = ntohs(udp_hdr_ptr->src_port);

  if (src_port_actual != src_port_expected)
  {
    printf("src_port actual result: %d expected result: %d\n",
          src_port_actual,
          src_port_expected);

    return action_skip;
  }

  uint16_t dst_port_expected = 10101;
  uint16_t dst_port_actual = ntohs(udp_hdr_ptr->dst_port);

  if (dst_port_actual != dst_port_expected)
  {
    printf("dst posrt actual result: %d expected result: %d\n",
            dst_port_actual,
            dst_port_expected);
    return action_skip;
  }

  uint16_t udp_hdr_total_length_expected = 46;
  uint16_t udp_hdr_total_length_actual = ntohs(udp_hdr_ptr->total_length);

  if (udp_hdr_total_length_actual != udp_hdr_total_length_expected)
  {
    printf("udp hdr total lenth actual result: %d expected result: %d\n",
           udp_hdr_total_length_actual,
           udp_hdr_total_length_expected);
    return action_skip;
  }

  uint16_t udp_hdr_checksum_expected = 0xE6B4;
  uint16_t udp_hdr_checksum_actual = ntohs(udp_hdr_ptr->checksum);

  if (udp_hdr_checksum_actual != udp_hdr_checksum_expected)
  {
    printf("udp hdr checksum actual result: 0x%X expected result: 0x%X\n",
          udp_hdr_checksum_actual,
          udp_hdr_checksum_expected);
    return action_skip;
  }

  uint16_t data_size_expected = 38;
  uint16_t data_size_actual = packet_desc_ptr->data_size;

  if (data_size_actual != data_size_expected)
  {
    printf("data size actual result: %d expected result: %d\n",
          data_size_actual,
          data_size_expected);
    return action_skip;
  }

  uint8_t* data = packet_desc_ptr->data;

  if (data == NULL)
  {
    printf("data == NULL\n");
    return action_skip;
  }

  int data_comp = data[0]  == 0x0A &&
                  data[1]  == 0x20 &&
                  data[2]  == 0x34 &&
                  data[37] == 0x14;

  if (!data_comp)
  {
    printf("data validation failed!");
    return action_skip;
  }

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

int vlan_ipv4_udp_test()
{
  return 1;
}

typedef struct __test_record_t
{
  const char* name;
  int (*func)();
} test_record_t;

const test_record_t TEST_RECORDS[] = {
  {"     ipv4_udp_test",      ipv4_udp_test},
  {"vlan_ipv4_udp_test", vlan_ipv4_udp_test}
};

const size_t TEST_COUT = sizeof(TEST_RECORDS) / sizeof(TEST_RECORDS[0]);

int main()
{
  printf("run tests...\n");

  int ret = 0;
  int test_result;
  size_t success_count = 0;
  size_t failed_count  = 0;

  for (size_t i = 0; i < TEST_COUT; ++i)
  {
    test_result = TEST_RECORDS[i].func();

    if (test_result == 0)
    {
      ++success_count;
    }
    else
    {
      ++failed_count;
      ret = 1;
    }

    printf("[%ld/%ld] %s: %s\n",
             i+1,
             TEST_COUT,
             TEST_RECORDS[i].name,
             test_result == 0 ? "SUCCESS" : "FAILED");
  }

  printf("Total test count: %ld success: %ld failed: %ld\n",
         TEST_COUT,
         success_count,
         failed_count);

  return ret;
}