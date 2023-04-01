#include "rate_calc.h"

namespace PcapAnalyzer
{

  void RateCalc::add(PcapTime pt)
  {
    while (!sequence_.empty())
    {
      if ((pt - sequence_.front()) <= PcapTime(1, 0))
      {
        break;
      }

      PcapTime t_begin = sequence_.front();

      on_rate_event(t_begin, size_);

      sequence_.pop_front();
      size_--;

      while (!sequence_.empty() && sequence_.front() == t_begin)
      {
        sequence_.pop_front();
        size_--;
      }
    }

    sequence_.push_back(pt);
    size_++;
  }

  void RateCalc::reset()
  {
    sequence_.clear();
    size_ = 0;
  }

  void RateCalc::stop()
  {
    if (!sequence_.empty())
    {
      on_rate_event(sequence_.front(), size_);
    }

    reset();
  }

}
