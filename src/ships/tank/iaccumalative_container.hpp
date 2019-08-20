//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_IACC_CONTAINER_HPP
#define DSCS_IACC_CONTAINER_HPP
#include "../component.hpp"

class iaccumalative_container : public icomponent {
public:
  iaccumalative_container(double max_capacity, double mass)
      : icomponent(mass), MAX_CAPACITY(max_capacity) {}

  void action() override {}

  double mass() const override { return icomponent::mass() + fuel_mass; }

  virtual double pull(double q) {
    auto out = (q < fuel_mass) ? q : fuel_mass;
    fuel_mass -= out;
    return out;
  }

  virtual void push(double q) {
    fuel_mass += q;
    fuel_mass = (fuel_mass < MAX_CAPACITY) ? fuel_mass : MAX_CAPACITY;
  }

  virtual double state() { return fuel_mass; }

protected:
  double fuel_mass{0};
  double const MAX_CAPACITY;
};

using piacc_container = std::shared_ptr<iaccumalative_container>;

#endif // DSCS_IACC_CONTAINER_HPP
