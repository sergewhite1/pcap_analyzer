extern "C"
{
  #include "pcap_analyzer.h"
}

#include <stdio.h>
#include <string.h>
#include <iostream>

action_t packet_process_example(const packet_desc_t* packet_desc_ptr)
{
  return action_copy;
}

int main(int argc, char* argv[])
{
  std::cout << "Hello!" << std::endl;
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

  return process_pcap_file(src_filename,
                           dst_filename,
                           packet_process_example);
}

