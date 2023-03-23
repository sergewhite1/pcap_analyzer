#include <chrono>
#include <stdio.h>
#include <string.h>
#include <unordered_map>

#include "utils/pcap_time.h"
#include "utils/tp_to_str.h"

std::unordered_map<char, int> d;

extern "C"
{
  #include "pcap_analyzer_lib/include/pcap_analyzer.h"
}

#include <iostream>

action_t packet_process_example(const packet_desc_t* packet_desc_ptr)
{
  std::cout << "index: " << packet_desc_ptr->index << std::endl;

  PcapAnalyzer::PcapTime pt(packet_desc_ptr);

  std::cout << "pt.seconds() =" << pt.seconds() << std::endl;
  std::cout << "pt.ms_or_ns()=" << pt.ms_or_ns() << std::endl;
  std::cout << "pt.to_strin()=" << pt.to_string() << std::endl;

  return action_skip;
}

int main(int argc, char* argv[])
{
  std::cout << "hello!!!!" << std::endl;

  auto t = std::chrono::system_clock::now();
  std::cout << "Start Time: " << PcapAnalyzer::tp_to_str(t) << std::endl;

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
  const char* dst_filename = nullptr;

  return process_pcap_file(src_filename,
                           dst_filename,
                           packet_process_example);

}

