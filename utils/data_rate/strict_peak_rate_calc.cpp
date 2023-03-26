#include "strict_peak_rate_calc.h"

namespace PcapAnalyzer
{

  StrictPeakRateCalc::StrictPeakRateCalc(const char* obj_name)
    : obj_name_(obj_name)
  {}

  void StrictPeakRateCalc::add(PcapTime pt)
  {
    while (!sequence_.empty())
    {
      if ((pt - sequence_.front()) <= PcapTime(1, 0))
      {
        break;
      }

      if (peak_pps_ < size_)
      {
        peak_pps_ = size_;
        peak_pps_time_begin_ = sequence_.front();
      }

      sequence_.pop_front();
      size_--;
    }

    sequence_.push_back(pt);
    size_++;
  }

  void StrictPeakRateCalc::stop()
  {
    if (!sequence_.empty())
    {
      if (peak_pps_ < size_)
      {
        peak_pps_ = size_;
        peak_pps_time_begin_ = sequence_.front();

        sequence_.pop_front();
        size_--;
      }
    }
  }

  int StrictPeakRateCalc::packets_per_second() const
  {
    return peak_pps_;
  }

  PcapTime StrictPeakRateCalc::peak_pps_time_begin() const
  {
    return peak_pps_time_begin_;
  }

  void StrictPeakRateCalc::reset()
  {
    sequence_.clear();
    size_ = 0;

    peak_pps_ = 0;
    peak_pps_time_begin_.set(0, 0);
  }

} // namespace PcapAnalyzer
