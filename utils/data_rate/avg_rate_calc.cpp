#include "avg_rate_calc.h"

namespace PcapAnalyzer
{
  AvgRateCalc::AvgRateCalc(const char* name, int threshold)
    : name_(name),
      threshold_(threshold)
  {}

  int AvgRateCalc::avg() const
  {
    if (count_)
    {
      return sum_ / count_;
    }

    return 0;
  }

  void AvgRateCalc::on_rate_event(PcapTime, int rate)
  {
    if (rate >= threshold_)
    {
      // TODO overflow check
      sum_ += rate;
      count_++;
    }
  }

  void AvgRateCalc::reset()
  {
    RateCalc::reset();

    sum_   = 0;
    count_ = 0;
  }

  void AvgRateCalc::reset(int new_threshold)
  {
    reset();
    threshold_ = new_threshold;
  }

}