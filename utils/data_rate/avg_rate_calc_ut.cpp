#include "utils/ut.h"

#include "utils/pcap_time.h"
#include "utils/data_rate/strict_peak_rate_calc.h"
#include "utils/data_rate/avg_rate_calc.h"

int run_AvgRateCalc_ut()
{
  UT_PRINT_START();

  using namespace PcapAnalyzer;

  int ret = 0;

  const PcapTime sequence[] = {

    PcapTime(1,0), PcapTime(1,1), PcapTime(1,2), PcapTime(1,3), PcapTime(1,4),
    PcapTime(1,5), PcapTime(1,6), PcapTime(1,7), PcapTime(1,8), PcapTime(1,9),
    PcapTime(2,0), PcapTime(2,2), PcapTime(2,4), PcapTime(2,6), PcapTime(2,8),
    PcapTime(3,0), PcapTime(3,3), PcapTime(3,6), PcapTime(3,9), PcapTime(10,0)
  };

  const size_t N = sizeof(sequence) / sizeof(sequence[0]);

  // Step 1. Calculate Peak Rate

  StrictPeakRateCalc peakRateCalc("test");

  for (size_t i = 0; i < N; ++i)
  {
    peakRateCalc.add(sequence[i]);
  }

  peakRateCalc.stop();

  int peak_rate = peakRateCalc.packets_per_second();

  CHECK_EQUAL(peak_rate, 11);
  CHECK_EQUAL(peakRateCalc.peak_pps_time_begin(), PcapTime(1, 0));

  // Step 2. Calcurate Average according to threshold rate

  AvgRateCalc avgRateCalc("test", 11);

  for (size_t i = 0; i < N; ++i)
  {
    avgRateCalc.add(sequence[i]);
  }

  avgRateCalc.stop();

  CHECK_EQUAL(avgRateCalc.avg(), 11);

  avgRateCalc.reset(8);

  for (size_t i = 0; i < N; ++i)
  {
    avgRateCalc.add(sequence[i]);
  }

  avgRateCalc.stop();

  CHECK_EQUAL(avgRateCalc.avg(), 9);

  UT_PRINT_RESULT();

  return ret;
}

int main()
{
  return run_AvgRateCalc_ut();
}