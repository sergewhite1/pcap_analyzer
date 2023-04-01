#include <iostream>
#include <vector>

#include "rate_calc.h"
#include "utils/ut.h"


class RateCalcTest1 : public PcapAnalyzer::RateCalc
{
  public:
    using vector_t = std::vector<std::pair<PcapAnalyzer::PcapTime, int>>;

    virtual void on_rate_event(PcapAnalyzer::PcapTime pt, int rate) override;

    int call_index() const { return call_index_; }

    vector_t& vec() { return vec_; }

  private:
    int call_index_ = 0;

    vector_t vec_;
};

void RateCalcTest1::on_rate_event(PcapAnalyzer::PcapTime pt, int rate)
{
//  std::cout << "RateCalcTest1::on_rate_event"
//            << " pt=" << pt
//            << " rate=" << rate << " pps" << std::endl;

  call_index_++;

  vec_.push_back(std::make_pair(pt, rate));
}

int run_RateCalc_ut()
{
  UT_PRINT_START();

  using namespace PcapAnalyzer;

  int ret = 0;

  RateCalcTest1 rateCalc;

  // Smoke test ================================================================

  rateCalc.add(PcapTime(1,0)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,1)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,2)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,3)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,4)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,5)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,6)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,7)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,8)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(1,9)); CHECK_EQUAL(rateCalc.call_index(), 0);
  rateCalc.add(PcapTime(2,0)); CHECK_EQUAL(rateCalc.call_index(), 0);

  rateCalc.add(PcapTime(2,2)); CHECK_EQUAL(rateCalc.call_index(), 2);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(1,0));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 11);

  CHECK_EQUAL(rateCalc.vec().at(1).first, PcapTime(1,1));
  CHECK_EQUAL(rateCalc.vec().at(1).second, 10);

  rateCalc.vec().clear();

  rateCalc.add(PcapTime(2,4)); CHECK_EQUAL(rateCalc.call_index(), 4);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(1,2));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 10);

  CHECK_EQUAL(rateCalc.vec().at(1).first, PcapTime(1,3));
  CHECK_EQUAL(rateCalc.vec().at(1).second, 9);

  rateCalc.vec().clear();

  // Several messages in one timestamp test ====================================

  rateCalc.add(PcapTime(4,2)); CHECK_EQUAL(rateCalc.call_index(), 13);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(1,4));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 9);

  CHECK_EQUAL(rateCalc.vec().at(1).first, PcapTime(1,5));
  CHECK_EQUAL(rateCalc.vec().at(1).second, 8);

  CHECK_EQUAL(rateCalc.vec().at(2).first, PcapTime(1,6));
  CHECK_EQUAL(rateCalc.vec().at(2).second, 7);

  CHECK_EQUAL(rateCalc.vec().at(3).first, PcapTime(1,7));
  CHECK_EQUAL(rateCalc.vec().at(3).second, 6);

  CHECK_EQUAL(rateCalc.vec().at(4).first, PcapTime(1,8));
  CHECK_EQUAL(rateCalc.vec().at(4).second, 5);

  CHECK_EQUAL(rateCalc.vec().at(5).first, PcapTime(1,9));
  CHECK_EQUAL(rateCalc.vec().at(5).second, 4);

  CHECK_EQUAL(rateCalc.vec().at(6).first, PcapTime(2,0));
  CHECK_EQUAL(rateCalc.vec().at(6).second, 3);

  CHECK_EQUAL(rateCalc.vec().at(7).first, PcapTime(2,2));
  CHECK_EQUAL(rateCalc.vec().at(7).second, 2);

  CHECK_EQUAL(rateCalc.vec().at(8).first, PcapTime(2,4));
  CHECK_EQUAL(rateCalc.vec().at(8).second, 1);

  rateCalc.vec().clear();

  rateCalc.add(PcapTime(4,2)); CHECK_EQUAL(rateCalc.call_index(), 13);
  rateCalc.add(PcapTime(4,2)); CHECK_EQUAL(rateCalc.call_index(), 13);

  rateCalc.add(PcapTime(7,0)); CHECK_EQUAL(rateCalc.call_index(), 14);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(4,2));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 3);

  CHECK_EQUAL(rateCalc.vec().size(), 1);

  rateCalc.vec().clear();

  // At the end of sequence test ==============================================

  rateCalc.add(PcapTime(7,3)); CHECK_EQUAL(rateCalc.call_index(), 14);
  rateCalc.add(PcapTime(7,7)); CHECK_EQUAL(rateCalc.call_index(), 14);

  rateCalc.stop(); CHECK_EQUAL(rateCalc.call_index(), 15);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(7,0));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 3);

  rateCalc.vec().clear();

  // reset test ===============================================================

  rateCalc.reset();

  rateCalc.add(PcapTime(10,0)); CHECK_EQUAL(rateCalc.call_index(), 15);

  rateCalc.reset();

  rateCalc.add(PcapTime(0,1)); CHECK_EQUAL(rateCalc.call_index(), 15);

  rateCalc.stop(); CHECK_EQUAL(rateCalc.call_index(), 16);

  CHECK_EQUAL(rateCalc.vec().at(0).first, PcapTime(0,1));
  CHECK_EQUAL(rateCalc.vec().at(0).second, 1);

  UT_PRINT_RESULT();

  return ret;
}

int main()
{
  return run_RateCalc_ut();
}
