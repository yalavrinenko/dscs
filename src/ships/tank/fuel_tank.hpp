//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_FUEL_TANK_HPP
#define DSCS_FUEL_TANK_HPP
#include "../component.hpp"

class fuel_tank: public icomponent{
public:
  fuel_tank(double fuel, double mass): icomponent(mass), fuel_mass(fuel){
  }

  void action() override {
  }

  double mass() const override { return icomponent::mass() + fuel_mass; }

  double pull(double q) {
     auto out = (q  < fuel_mass) ? q : fuel_mass;
  }

protected:
  double fuel_mass;
};

#endif // DSCS_FUEL_TANK_HPP
