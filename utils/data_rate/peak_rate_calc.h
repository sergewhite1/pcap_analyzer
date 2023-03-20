#ifndef PEAK_RATE_CALC_H_
#define PEAK_RATE_CALC_H_

#include "utils/pcap_time.h"

namespace PcapAnalyzer
{
  class PeakRateCalc
  {
    public:
      virtual ~PeakRateCalc() = default;

      virtual void add(PcapTime pt) =0;
      virtual void stop() {}
      virtual int packets_per_second() const =0;
      virtual PcapTime peak_pps_time_begin() const =0;
      virtual void reset() =0;
  };
}

#endif