#ifndef PCAP_ANALYZER_AVG_RATE_COLLECTION_H_
#define PCAP_ANALYZER_AVG_RATE_COLLECTION_H_

#include <memory>
#include <vector>

#include "avg_rate_calc.h"

namespace PcapAnalyzer
{

  class AvgRateCollection
  {
    public:
      AvgRateCollection();

      int peak_rate() const { return peak_rate_; }
      void reset(int new_peak_rate);
      void add(PcapTime pt);
      void stop();
      void print() const;

      double get_rel_threshold_by_index(size_t index) const;

      int get_abs_threshold_by_index(size_t index) const
      {
        return v_.at(index)->threshold();
      }

      int get_avg_rate_by_index(size_t index) const
      {
        return v_.at(index)->avg();
      }

    private:
      int peak_rate_ = 0;
      std::vector<std::unique_ptr<AvgRateCalc>> v_;

  };

}

#endif