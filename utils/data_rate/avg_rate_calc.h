#ifndef PCAP_ANALYZER_AVG_RATE_CALC_H_
#define PCAP_ANALYZER_AVG_RATE_CALC_H_

#include "rate_calc.h"

namespace PcapAnalyzer{

  class AvgRateCalc : public RateCalc
  {
    public:
      AvgRateCalc(const char* name, int threshold);

    // RateCalc Interface
    protected:
      virtual void on_rate_event(PcapTime pt, int rate) override;

    public:
      virtual void reset() override;
      void reset(int new_threshold);

    public:
      std::string name() const { return name_; }
      int threshold() const {return threshold_;}

      int avg() const;

    private:
      std::string name_;
      int threshold_ = 0;

      int sum_   = 0;
      int count_ = 0;
  };

}

#endif