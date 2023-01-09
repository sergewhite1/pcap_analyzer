// PCAP Time

#ifndef PCAP_TIME_H_
#define PCAP_TIME_H_

#include <cstdint>
#include <ctime>
#include <string>

extern "C"
{
  #include "pcap_analyzer_lib/include/pcap_analyzer.h"
}

namespace PcapAnalyzer
{

class PcapTime
{
public:
  PcapTime() = default;

  explicit PcapTime(const packet_desc_t* packet_desc_ptr)
  : seconds_ (packet_desc_ptr->packet_record_ptr->timestamp_seconds),
    ms_or_ns_(packet_desc_ptr->packet_record_ptr->timestamp_ms_or_ns)
  {}

  PcapTime(time_t __seconds, uint32_t __ms_or_ns)
  : seconds_ (__seconds),
    ms_or_ns_(__ms_or_ns)
  {}

  time_t   seconds()  const { return seconds_;  }
  uint32_t ms_or_ns() const { return ms_or_ns_; }

  void set(time_t __seconds, uint32_t __ms_or_ns)
  {
    seconds_  = __seconds;
    ms_or_ns_ = __ms_or_ns;
  }

  std::string to_string() const;

private:
  time_t   seconds_  = 0;
  uint32_t ms_or_ns_ = 0;
};

} // namespace PcapAnalyzer

#endif // ! PCAP_TIME_H_
