//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_BATTERY_HPP
#define DSCS_BATTERY_HPP
#include "iaccumalative_container.hpp"

class battery: public iaccumalative_container {
public:
  battery(double max_capacity, double mass)
      : iaccumalative_container(max_capacity, mass) {}
};

#endif // DSCS_BATTERY_HPP
