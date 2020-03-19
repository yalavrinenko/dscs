//
// Created by yalavrinenko on 21.08.2019.
//

#ifndef DSCS_FUEL_ADAPTER_HPP
#define DSCS_FUEL_ADAPTER_HPP
#include "fuel_tank.hpp"
#include "icontainer_adapter.hpp"
using namespace std::string_literals;
class fuel_tank_line: public itank_adapter<fuel_tank>{
public:
  fuel_tank_line(double mass, double input_power, double output_power,
                 std::string const &name, plogger logger)
      : itank_adapter(mass, input_power, output_power, "Fuel line "s + name,
                      std::move(logger)) {}
};
using pfuel_tank_line = std::shared_ptr<fuel_tank_line>;


#endif // DSCS_FUEL_ADAPTER_HPP
