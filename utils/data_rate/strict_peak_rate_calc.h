#ifndef STRICT_PEAK_RATE_CALC_H_
#define STRICT_PEAK_RATE_CALC_H_

#include "peak_rate_calc.h"

#include <list>

namespace PcapAnalyzer
{
  class StrictPeakRateCalc : public PeakRateCalc
  {
    public:

      StrictPeakRateCalc(const char* obj_name);

      virtual void add(PcapTime pt) override;
      virtual void stop() override;
      virtual int packets_per_second() const override;
      virtual PcapTime peak_pps_time_begin() const override;
      virtual void reset() override;

      virtual std::string obj_name() const override
      {
        return obj_name_;
      }

      virtual std::string algorithm_name() const override
      {
        return "Strict";
      }

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

      std::string obj_name_;
    };
}  // namespace PcapAnalyzer

#endif // ! STRICT_PEAK_RATE_CALC_H_
