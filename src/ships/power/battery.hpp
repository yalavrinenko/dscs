//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_BATTERY_HPP
#define DSCS_BATTERY_HPP
#include "../component.hpp"
class battery: public icomponent {
public:
  battery(double capacity, double mass): icomponent(mass), max_charge_(capacity){
  }

  virtual void charge(double dq){
    charge_ += dq;
    if (charge_ > max_charge_)
      charge_ = max_charge_;
  }

  double use(double rq){
    if (charge_ < rq){
      auto to_ret = charge_;
      charge_ = 0;
      return to_ret;
    } else {
      charge_ -= rq;
      return rq;
    }
  }

  void action() override {
  }

protected:
  double max_charge_;

private:
  double charge_{0.0};
};

#endif // DSCS_BATTERY_HPP
