#include "avg_rate_collection.h"

#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <sstream>

namespace PcapAnalyzer
{

  static const double COEFFS[] = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5,
                                  0.6, 0.7, 0.8, 0.9, 1.0};

  static constexpr size_t COEFFS_COUNT = sizeof(COEFFS) / sizeof(COEFFS[0]);

  AvgRateCollection::AvgRateCollection()
  {
    for (size_t i = 0; i < COEFFS_COUNT; i++)
    {
      std::stringstream ss;

      ss << "threshold="
         << std::setprecision(1) << std::fixed << COEFFS[i] << "*peak_rate";

      v_.emplace_back(new AvgRateCalc(ss.str().c_str(), 0));
    }
  }

  void AvgRateCollection::reset(int new_peak_rate)
  {
    peak_rate_ = new_peak_rate;

    for (size_t i = 0; i < COEFFS_COUNT; i++)
    {
      v_.at(i)->reset(COEFFS[i] * peak_rate_);
    }
  }

  void AvgRateCollection::add(PcapTime pt)
  {
    for (auto& elem : v_)
    {
      elem->add(pt);
    }
  }

  void AvgRateCollection::stop()
  {
    for (auto& elem : v_)
    {
      elem->stop();
    }
  }

  void AvgRateCollection::print() const
  {
    static const std::string L80(80, '-');

    std::cout << "Peak Rate: " << peak_rate_ << std::endl;

    std::cout << L80 << std::endl;

    std::cout << std::setw(25) << "Rel Threshold|"
              << std::setw(25) << "Abs Threshold|"
              << std::setw(25) << "Avg Rate (pps)|" << std::endl;

    std::cout << L80 << std::endl;

    for (size_t i = 0; i < COEFFS_COUNT; i++)
    {
      std::cout << std::setw(24) << COEFFS[i] << "|"
                << std::setw(24) << v_.at(i)->threshold() << "|"
                << std::setw(24) << v_.at(i)->avg() << "|" << std::endl;
    }

    std::cout << L80 << std::endl;
  }

  double AvgRateCollection::get_rel_threshold_by_index(size_t index) const
  {
    if (index < COEFFS_COUNT)
    {
      return COEFFS[index];
    }

    std::stringstream ss;
    ss << "Out of Range in AvgRateCollection::get_rel_threshold_by_index"
       << " index=" << index << " but maximun is " << COEFFS_COUNT - 1;

    throw std::out_of_range(ss.str());
  }

}