//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_IACC_CONTAINER_HPP
#define DSCS_IACC_CONTAINER_HPP
#include "../component.hpp"

class iaccumalative_container : public icomponent {
public:
  iaccumalative_container(double max_capacity, double mass, double mass_pp, std::string name,
                          plogger logger)
      : icomponent(mass, std::move(name), std::move(logger)),
        MAX_CAPACITY_(max_capacity),
        mass_pp_(mass_pp){}

  void action() override {
  }

  [[nodiscard]]
  double mass() const override { return icomponent::mass() + fuel_mass_ * mass_pp_; }

  virtual double pull(double q) {
    auto out = (q < fuel_mass_) ? q : fuel_mass_;
    fuel_mass_ -= out;
    consumption_ += -out;
    return out;
  }

  virtual double push(double q) {
    auto old_mass = fuel_mass_;
    q = (fuel_mass_ + q > MAX_CAPACITY_) ? MAX_CAPACITY_ - fuel_mass_ : q;
    fuel_mass_ += q;

    consumption_ += q;

    return fuel_mass_ - old_mass;
  }

  virtual double size() { return fuel_mass_; }

  void log_action() const override {
    this->logger()->log(this->name() + ":");
    this->logger()->log("\t(left/total/consumption)", "(", fuel_mass_, "/", MAX_CAPACITY_, "/", consumption_, ")");
    this->logger()->log("\tMass:", this->mass());
  }

  double consumption() const {
    auto consumption_value = consumption_;
    consumption_ = 0.0;
    return consumption_value;
  }

  double capacity() const { return fuel_mass_;  }

  double max_capacity() const { return MAX_CAPACITY_; }
protected:
  double fuel_mass_{0};
  double const MAX_CAPACITY_;
  mutable double consumption_{0};
  double mass_pp_;

};

using piacc_container = std::shared_ptr<iaccumalative_container>;

#endif // DSCS_IACC_CONTAINER_HPP
