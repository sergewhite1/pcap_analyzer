#include "pcap_time.h"

#include <cstring>
#include <iomanip>
#include <sstream>

namespace PcapAnalyzer 
{

  PcapTime::PcapTime(int year, int month, int day,
                     int hour, int minute, int second, int ms_or_ns)
  {
    struct tm tm_obj;

    memset(&tm_obj, 0, sizeof(tm_obj));

    tm_obj.tm_year = year - 1900;
    tm_obj.tm_mon  = month - 1;
    tm_obj.tm_mday = day;
    tm_obj.tm_hour = hour;
    tm_obj.tm_min  = minute;
    tm_obj.tm_sec  = second;

    seconds_  = timegm(&tm_obj); // mktime uses local time zone
    ms_or_ns_ = ms_or_ns;
  }


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

  bool PcapTime::operator<(const PcapTime& rhs) const
  {
    if (seconds_ < rhs.seconds_)
    {
      return true;
    }

    if (seconds_ == rhs.seconds_)
    {
      return ms_or_ns_ < rhs.ms_or_ns_;
    }

    return false;
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

  std::ostream& operator<< (std::ostream& stream, const PcapTime& pt)
  {
    stream << pt.to_string();
    return stream;
  }

}  // namespace PcapAnalyzer
