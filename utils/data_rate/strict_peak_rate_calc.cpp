#include "strict_peak_rate_calc.h"

namespace PcapAnalyzer
{

  StrictPeakRateCalc::StrictPeakRateCalc(const char* obj_name)
    : obj_name_(obj_name)
  {}

  void StrictPeakRateCalc::add(PcapTime pt)
  {
    int pop_count = 0;

    for (auto& elem : sequence_)
    {
      if (pt - elem.time <= PcapTime(1, 0))
      {
        elem.count++;
      }
      else
      {
        if (max_.count < elem.count )
        {
          max_ = elem;
        }
        pop_count++;
      }
    }

    for (int i = 0; i < pop_count; ++i)
    {
      sequence_.pop_front();
    }

    sequence_.emplace_back(1, pt);
  }

  void StrictPeakRateCalc::stop()
  {
    if (!sequence_.empty())
    {
      if (max_.count < sequence_.front().count)
      {
        max_ = sequence_.front();
        sequence_.pop_front();
      }
    }
  }

  int StrictPeakRateCalc::packets_per_second() const
  {
    return max_.count;
  }

  PcapTime StrictPeakRateCalc::peak_pps_time_begin() const
  {
    return max_.time;
  }

  void StrictPeakRateCalc::reset()
  {
    sequence_.clear();

    max_.count = 0;
    max_.time  = PcapTime(0, 0);
  }

} // namespace PcapAnalyzer
