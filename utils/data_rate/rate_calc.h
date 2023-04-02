#ifndef PCAP_ANALYZER_RATE_CALC_H_
#define PCAP_ANALYZER_RATE_CALC_H_

#include <list>

#include "utils/pcap_time.h"

namespace PcapAnalyzer
{
  class RateCalc
  {
    public:
      virtual ~RateCalc() = default;

      virtual void add(PcapTime pt);
      virtual void reset();
      virtual void stop();

    protected:
      virtual void on_rate_event(PcapTime pt, int rate) = 0;

    private:
      std::list<PcapTime> sequence_;
      int size_ = 0;
  };
}

#endif
