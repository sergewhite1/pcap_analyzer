#include "strict_peak_rate_calc.h"
#include "utils/pcap_time.h"
#include "utils/ut.h"

int run_StrictPeakRateCalc_ut()
{
  using namespace PcapAnalyzer;

  int ret = 0;

  StrictPeakRateCalc peakRateCalc("test");
  int packets_per_second = 0;
  PcapTime peak_pps_time_begin;

  // peak in the begin ========================================================

  peakRateCalc.add(PcapTime(0, 5));
  peakRateCalc.add(PcapTime(0, 6));
  peakRateCalc.add(PcapTime(0, 7));
  peakRateCalc.add(PcapTime(0, 8));
  peakRateCalc.add(PcapTime(0, 9));
  peakRateCalc.add(PcapTime(1, 0));
  peakRateCalc.add(PcapTime(1, 1));
  peakRateCalc.add(PcapTime(1, 2));
  peakRateCalc.add(PcapTime(1, 3));
  peakRateCalc.add(PcapTime(1, 4));
  peakRateCalc.add(PcapTime(1, 5));

  peakRateCalc.add(PcapTime(3, 1));
  peakRateCalc.add(PcapTime(3, 3));
  peakRateCalc.add(PcapTime(3, 6));
  peakRateCalc.add(PcapTime(4, 2));
  peakRateCalc.add(PcapTime(5, 0));
  peakRateCalc.add(PcapTime(7, 0));

  packets_per_second = peakRateCalc.packets_per_second();

  CHECK_EQUAL(packets_per_second, 11);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(0, 5));

  // peak in the middle =======================================================

  peakRateCalc.reset();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 0);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(0, 0));

  peakRateCalc.add(PcapTime(0, 1));
  peakRateCalc.add(PcapTime(0, 3));
  peakRateCalc.add(PcapTime(0, 6));
  peakRateCalc.add(PcapTime(1, 2));

  peakRateCalc.add(PcapTime(3, 5));
  peakRateCalc.add(PcapTime(3, 6));
  peakRateCalc.add(PcapTime(3, 7));
  peakRateCalc.add(PcapTime(3, 8));
  peakRateCalc.add(PcapTime(3, 9));
  peakRateCalc.add(PcapTime(4, 0));
  peakRateCalc.add(PcapTime(4, 1));
  peakRateCalc.add(PcapTime(4, 2));
  peakRateCalc.add(PcapTime(4, 3));
  peakRateCalc.add(PcapTime(4, 4));
  peakRateCalc.add(PcapTime(4, 5));

  peakRateCalc.add(PcapTime(5, 0));
  peakRateCalc.add(PcapTime(7, 0));

  peakRateCalc.stop();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 11);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(3, 5));

  // peak in the end ==========================================================

  peakRateCalc.reset();

  peakRateCalc.add(PcapTime(0, 1));
  peakRateCalc.add(PcapTime(0, 3));
  peakRateCalc.add(PcapTime(0, 6));
  peakRateCalc.add(PcapTime(1, 2));
  peakRateCalc.add(PcapTime(2, 0));
  peakRateCalc.add(PcapTime(3, 0));

  peakRateCalc.add(PcapTime(4, 5));
  peakRateCalc.add(PcapTime(4, 6));
  peakRateCalc.add(PcapTime(4, 7));
  peakRateCalc.add(PcapTime(4, 8));
  peakRateCalc.add(PcapTime(4, 9));
  peakRateCalc.add(PcapTime(5, 0));
  peakRateCalc.add(PcapTime(5, 1));
  peakRateCalc.add(PcapTime(5, 2));
  peakRateCalc.add(PcapTime(5, 3));
  peakRateCalc.add(PcapTime(5, 4));
  peakRateCalc.add(PcapTime(5, 5));
  peakRateCalc.add(PcapTime(5, 6));

  peakRateCalc.stop();

  packets_per_second = peakRateCalc.packets_per_second();
  CHECK_EQUAL(packets_per_second, 11);

  peak_pps_time_begin = peakRateCalc.peak_pps_time_begin();
  CHECK_EQUAL(peak_pps_time_begin, PcapTime(4, 5));

  return ret;
}

int main()
{
  return run_StrictPeakRateCalc_ut();
}
