#include "pcap_time.h"

#include <iomanip>
#include <sstream>

namespace PcapAnalyzer 
{
  std::string PcapTime::to_string() const
  {
    // Output format: YYYY-MM-DD HH:MM:SS.ms_or_ns

    std::stringstream ss;

    struct tm* tm_ptr = gmtime(&seconds_);

    ss << tm_ptr->tm_year + 1900 << "-"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_mon + 1 << "-"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_mday << " "
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_hour << ":"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_min << ":"
       << std::setfill('0') << std::setw(2) << tm_ptr->tm_sec << "."
       << std::setfill('0') << std::setw(6) << ms_or_ns_;

    return ss.str();
  }

  PcapTime PcapTime::operator-(const PcapTime& rhs) const
  {
    // formula: *this - rhs
    // if *this <= rhs

    if (seconds_ < rhs.seconds_)
    {
      // *this > rhs
      return PcapTime();
    }
    else if (seconds_ == rhs.seconds_)
    {
      if (ms_or_ns_ <= rhs.ms_or_ns_)
      {
        // *this >= rhs
        return PcapTime();
      }
      else
      {
        return PcapTime(0, ms_or_ns_ - rhs.ms_or_ns_);
      }
    }

    time_t new_seconds = seconds_ - rhs.seconds_;

    if (ms_or_ns_ < rhs.ms_or_ns_)
    {
      return PcapTime(new_seconds - 1, rhs.ms_or_ns_ - ms_or_ns_);
    }

    return PcapTime(new_seconds, ms_or_ns_ - rhs.ms_or_ns_);
  }
}  // namespace PcapAnalyzer
