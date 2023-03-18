#ifndef PEAK_RATE_CALC_H_
#define PEAK_RATE_CALC_H_

#include <list>

#include "pcap_time.h"

namespace PcapAnalyzer
{

class PeakRateCalc
{
public:

  void add(PcapTime pt);
  void stop();
  int packets_per_second();
  PcapTime peak_pps_time_begin();
  void reset();

private:

  class PacketData
  {
    public:
      PacketData() = default;

      PacketData(int __count, PcapTime __time)
      : count(__count),
        time(__time)
      {}

      int count = 0;
      PcapTime time;
  };

  std::list<PacketData> sequence_;
  PacketData max_;
};

}

#endif
