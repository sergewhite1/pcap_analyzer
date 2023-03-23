#include "sw_peak_rate_calc.h"

namespace PcapAnalyzer
{

  SWPeakRateCalc::SWPeakRateCalc(const char* obj_name)
    : obj_name_(obj_name)
  {}

  void SWPeakRateCalc::add(PcapTime pt)
  {
    if (pt.seconds() == curr_time_begin_.seconds())
    {
      curr_count_++;
    }
    else
    {
      if (max_count_ < curr_count_)
      {
        max_count_  = curr_count_;
        time_begin_ = curr_time_begin_;
      }

      curr_count_      = 1;
      curr_time_begin_ = pt;
    }
  }

  void SWPeakRateCalc::stop()
  {
    if (max_count_ < curr_count_)
    {
      max_count_  = curr_count_;
      time_begin_ = curr_time_begin_;
    }
  }

  int SWPeakRateCalc::packets_per_second() const
  {
    return max_count_;
  }

  PcapTime SWPeakRateCalc::peak_pps_time_begin() const
  {
    return time_begin_;
  }

  void SWPeakRateCalc::reset()
  {
    curr_count_ = 0;
    curr_time_begin_.set(0, 0);

    max_count_ = 0;
    time_begin_.set(0, 0);
  }

}  // namespace PcapAnalyzer
