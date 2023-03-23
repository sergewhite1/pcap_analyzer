#ifndef PCAP_ANALYZER_TP_TO_STR_H_
#define PCAP_ANALYZER_TP_TO_STR_H_

// time point to string conversion

#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace PcapAnalyzer{

  template <typename time_point_t>
  std::string tp_to_str(time_point_t tp)
  {
    using clock = typename time_point_t::clock;
    std::time_t tt = clock::to_time_t(tp);

    struct tm* tm_ptr = gmtime(&tt);

    std::stringstream ss;

    ss << tm_ptr->tm_year + 1900 << "-"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_mon + 1 << "-"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_mday << " "
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_min << ":"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_sec
       << " (GMT)";

    return ss.str();
  }

}

#endif  // ! PCAP_ANALYZER_TIME_TO_STR_H_