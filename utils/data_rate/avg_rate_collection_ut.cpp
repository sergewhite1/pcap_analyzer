#include <list>

#include "utils/ut.h"
#include "utils/data_rate/strict_peak_rate_calc.h"
#include "utils/data_rate/avg_rate_collection.h"

int run_AvgRateCollection_ut()
{
  UT_PRINT_START();

  using namespace PcapAnalyzer;

  int ret = 0;

  std::list<PcapTime> sequence;

  for (int pps = 1; pps <= 100; pps ++)
  {
    for (int count = 0; count < pps; count++)
    {
      sequence.emplace_back(pps * 2, 0);
    }
  }

  StrictPeakRateCalc prc("Strict_Peak_Rate_Calculator");

  for (const auto elem : sequence)
  {
    prc.add(elem);
  }

  prc.stop();

  CHECK_EQUAL(prc.packets_per_second(), 100);
  CHECK_EQUAL(prc.peak_pps_time_begin(), PcapTime(200, 0));

  AvgRateCollection arc;

  arc.reset(prc.packets_per_second());

  for (const auto elem : sequence)
  {
    arc.add(elem);
  }

  arc.stop();

  arc.print();

  CHECK_EQUAL(arc.get_rel_threshold_by_index(0),  0.0);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(0),    0);
  CHECK_EQUAL(arc.get_avg_rate_by_index(0),        50);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(1),  0.1);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(1),   10);
  CHECK_EQUAL(arc.get_avg_rate_by_index(1),        55);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(2),  0.2);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(2),   20);
  CHECK_EQUAL(arc.get_avg_rate_by_index(2),        60);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(2),  0.2);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(2),   20);
  CHECK_EQUAL(arc.get_avg_rate_by_index(2),        60);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(3),  0.3);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(3),   30);
  CHECK_EQUAL(arc.get_avg_rate_by_index(3),        65);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(4),  0.4);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(4),   40);
  CHECK_EQUAL(arc.get_avg_rate_by_index(4),        70);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(5),  0.5);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(5),   50);
  CHECK_EQUAL(arc.get_avg_rate_by_index(5),        75);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(6),  0.6);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(6),   60);
  CHECK_EQUAL(arc.get_avg_rate_by_index(6),        80);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(7),  0.7);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(7),   70);
  CHECK_EQUAL(arc.get_avg_rate_by_index(7),        85);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(8),  0.8);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(8),   80);
  CHECK_EQUAL(arc.get_avg_rate_by_index(8),        90);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(9),  0.9);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(9),   90);
  CHECK_EQUAL(arc.get_avg_rate_by_index(9),        95);

  CHECK_EQUAL(arc.get_rel_threshold_by_index(10),  1.0);
  CHECK_EQUAL(arc.get_abs_threshold_by_index(10),  100);
  CHECK_EQUAL(arc.get_avg_rate_by_index(10),       100);

  UT_PRINT_RESULT();

  return ret;
}

int main()
{
  return run_AvgRateCollection_ut();
}