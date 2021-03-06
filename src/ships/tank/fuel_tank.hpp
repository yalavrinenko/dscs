//
// Created by yalavrinenko on 03.06.19.
//

#ifndef DSCS_FUEL_TANK_HPP
#define DSCS_FUEL_TANK_HPP

#include "iaccumalative_container.hpp"
using namespace std::string_literals;

static int fuel_tank_count = 0;

class fuel_tank : public iaccumalative_container {
public:
  fuel_tank(double max_capacity, double mass, double gas_mass, plogger logger)
      : iaccumalative_container(max_capacity, mass, gas_mass,
                                "fuel_tank_"s +
                                    std::to_string(fuel_tank_count++),
                                std::move(logger)) {}
};

struct fuel_type{
  using liquid = size_scale<1, 100>;
};

template <typename scale_type, typename fuel_type>
class fuel_tank_config {
public:
  static double constexpr mass = 0.01 * scale_type::scale;
  static double constexpr capacity = 1000 * scale_type::scale;
  static double constexpr fuel_mass = fuel_type::scale;
};

#endif // DSCS_FUEL_TANK_HPP
