#include "sw_peak_rate_calc.h"
#include "utils/ut.h"

int run_SWPeakRateCalc_ut()
{
  UT_PRINT_START();

  using namespace PcapAnalyzer;

  int ret = 0;

  SWPeakRateCalc peakRateCalc("test");
  int packets_per_second = 0;
  PcapTime peak_pps_time_begin;

  // Maximum in the middle in 1 window ========================================

  peakRateCalc.add(PcapTime(0, 5));
  peakRateCalc.add(PcapTime(1, 2));
  peakRateCalc.add(PcapTime(1, 3));

  peakRateCalc.add(PcapTime(2, 0));
  peakRateCalc.add(PcapTime(2, 1));
  peakRateCalc.add(PcapTime(2, 2));
  peakRateCalc.add(PcapTime(2, 3));
  peakRateCalc.add(PcapTime(2, 4));
  peakRateCalc.add(PcapTime(2, 5));
  peakRateCalc.add(PcapTime(2, 6));
  peakRateCalc.add(PcapTime(2, 7));
  peakRateCalc.add(PcapTime(2, 8));
  peakRateCalc.add(PcapTime(2, 9));
  peakRateCalc.add(PcapTime(3, 0));

  peakRateCalc.add(PcapTime(3, 5));

  peakRateCalc.stop();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 10);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(2, 0));

  // reset test ===============================================================

  peakRateCalc.reset();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 0);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(0, 0));

  // Maximum in the middle in 2 windows =======================================

  peakRateCalc.reset();

  peakRateCalc.add(PcapTime(0, 5));

  peakRateCalc.add(PcapTime(1, 5));
  peakRateCalc.add(PcapTime(1, 6));
  peakRateCalc.add(PcapTime(1, 7));
  peakRateCalc.add(PcapTime(1, 8));
  peakRateCalc.add(PcapTime(1, 9));
  peakRateCalc.add(PcapTime(2, 0));
  peakRateCalc.add(PcapTime(2, 1));
  peakRateCalc.add(PcapTime(2, 2));
  peakRateCalc.add(PcapTime(2, 3));
  peakRateCalc.add(PcapTime(2, 4));
  peakRateCalc.add(PcapTime(3, 0));

  peakRateCalc.add(PcapTime(3, 5));

  peakRateCalc.stop();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 5);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(1, 5));

  // Maximun in the end =======================================================

  peakRateCalc.reset();

  peakRateCalc.add(PcapTime(0, 5));
  peakRateCalc.add(PcapTime(1, 2));
  peakRateCalc.add(PcapTime(1, 3));

  peakRateCalc.add(PcapTime(2, 0));
  peakRateCalc.add(PcapTime(2, 1));
  peakRateCalc.add(PcapTime(2, 2));
  peakRateCalc.add(PcapTime(2, 3));
  peakRateCalc.add(PcapTime(2, 4));
  peakRateCalc.add(PcapTime(2, 5));
  peakRateCalc.add(PcapTime(2, 6));
  peakRateCalc.add(PcapTime(2, 7));
  peakRateCalc.add(PcapTime(2, 8));
  peakRateCalc.add(PcapTime(2, 9));

  peakRateCalc.stop();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 10);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(2, 0));

  UT_PRINT_RESULT();

  return ret;
}

int main()
{
  return run_SWPeakRateCalc_ut();
}
