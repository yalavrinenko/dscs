//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_BATTERY_HPP
#define DSCS_BATTERY_HPP
#include "iaccumalative_container.hpp"

using namespace std::string_literals;
static int battery_count = 0;

class battery: public iaccumalative_container {
public:
  battery(double max_capacity, double mass, plogger logger)
      : iaccumalative_container(max_capacity, mass, 0.0,
                                "battery_"s + std::to_string(battery_count++),
                                std::move(logger)) {}
};

#endif // DSCS_BATTERY_HPP
