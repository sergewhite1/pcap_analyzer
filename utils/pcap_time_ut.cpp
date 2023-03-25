#include <iostream>

#include "utils/pcap_time.h"

#include "utils/ut.h"

int run_PcapTime_ut()
{
  std::cout << "run_PcapTime_ut..." << std::endl;

  int ret = 0;

  // to_string test ===========================================================
  {
    PcapAnalyzer::PcapTime pt(1671625208, 123);

    std::string   actual_result = pt.to_string();
    std::string expected_result = "2022-12-21 12:20:08.000123";

    CHECK_EQUAL(actual_result, expected_result);
  }

  // human readable ctor test =================================================
  {
    PcapAnalyzer::PcapTime pt(2022, 12, 28, 12, 15, 30, 145);
    std::string   actual_result = pt.to_string();
    std::string expected_result = "2022-12-28 12:15:30.000145";

    CHECK_EQUAL(actual_result, expected_result);
  }

  // operator- test ===========================================================
  {
    PcapAnalyzer::PcapTime t1(100, 200);
    PcapAnalyzer::PcapTime t2( 50, 123);

    PcapAnalyzer::PcapTime t3 = t2 - t1; // t2 < t1
    CHECK_EQUAL(t3.seconds(),  0);
    CHECK_EQUAL(t3.ms_or_ns(), 0);

    t2.set(100, 200);  // t2 == t1
    t3 = t2 - t1;
    CHECK_EQUAL(t3.seconds(),  0);
    CHECK_EQUAL(t3.ms_or_ns(), 0);

    t2.set(100, 201);
    t3 = t2 - t1;
    CHECK_EQUAL(t3.seconds(),  0);
    CHECK_EQUAL(t3.ms_or_ns(), 1);

    t2.set(101, 202);
    t3 = t2 - t1;
    CHECK_EQUAL(t3.seconds(),  1);
    CHECK_EQUAL(t3.ms_or_ns(), 2);

    t2.set(101, 200);
    t3 = t2 - t1;
    CHECK_EQUAL(t3.seconds(),  1);
    CHECK_EQUAL(t3.ms_or_ns(), 0);

    t2.set(101, 199);
    t3 = t2 - t1;
    CHECK_EQUAL(t3.seconds(),  0);
    CHECK_EQUAL(t3.ms_or_ns(), 1);
  }

  // comp operator tests ======================================================
  {
    PcapAnalyzer::PcapTime t1(500, 500);
    PcapAnalyzer::PcapTime t2(700, 300);
    PcapAnalyzer::PcapTime t3(700, 300);
    PcapAnalyzer::PcapTime t4(800, 0);

    CHECK_FALSE (t1 == t2);
    CHECK_TRUE  (t1 <  t2);
    CHECK_TRUE  (t1 <= t2);
    CHECK_FALSE (t1 >  t2);
    CHECK_FALSE (t1 >= t2);

    CHECK_TRUE  (t2 == t3);
    CHECK_FALSE (t2 <  t3);
    CHECK_TRUE  (t2 <= t3);
    CHECK_FALSE (t2 >  t3);
    CHECK_TRUE  (t2 >= t3);

    CHECK_FALSE (t3 == t4);
    CHECK_TRUE  (t3 <  t4);
    CHECK_TRUE  (t3 <= t4);
    CHECK_FALSE (t3 >  t4);
    CHECK_FALSE (t3 >= t4);
  }

  if (ret == 0)
  {
    std::cout << "SUCCESS" << std::endl;
  }
  else
  {
    std::cout << "FAILED!" << std::endl;
  }

  //

  return ret;
}

int main()
{
  return run_PcapTime_ut();
}
