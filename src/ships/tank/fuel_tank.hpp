//
// Created by yalavrinenko on 03.06.19.
//

#ifndef DSCS_FUEL_TANK_HPP
#define DSCS_FUEL_TANK_HPP

#include "iaccumalative_container.hpp"

class fuel_tank: public iaccumalative_container{
public:
  fuel_tank(double max_capacity, double mass)
      : iaccumalative_container(max_capacity, mass) {}
};

#endif //DSCS_FUEL_TANK_HPP
