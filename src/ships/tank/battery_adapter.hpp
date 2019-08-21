//
// Created by yalavrinenko on 21.08.2019.
//

#ifndef DSCS_BATTERY_ADAPTER_HPP
#define DSCS_BATTERY_ADAPTER_HPP
#include "battery.hpp"
#include "icontainer_adapter.hpp"
using namespace std::string_literals;
class battery_line: public itank_adapter<battery>{
public:
  battery_line(double mass, double input_power, double output_power,
                 std::string name, plogger logger)
      : itank_adapter(mass, input_power, output_power, "Battery line "s + name,
                      std::move(logger)) {}
};
using pbattery_line = std::shared_ptr<battery_line>;
#endif // DSCS_BATTERY_ADAPTER_HPP
