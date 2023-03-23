#ifndef SW_PEAK_RATE_CALC_H_
#define SW_PEAK_RATE_CALC_H_

// SW Peak Rate Calculator (Slading Window)

#include "peak_rate_calc.h"

namespace PcapAnalyzer
{
  class SWPeakRateCalc : public PeakRateCalc
  {
    public:

      SWPeakRateCalc(const char* obj_name);

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
        return "Sliding_Window";
      }
    private:
      int      curr_count_   = 0;
      PcapTime curr_time_begin_;

      int      max_count_ = 0;
      PcapTime time_begin_;

      std::string obj_name_;
  };

} // namespace PcapAnalyzer


#endif
