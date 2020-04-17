//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_SMALL_HPP
#define DSCS_SMALL_HPP

#include "hull.hpp"
#include "engines/engine.hpp"
#include "power/reactor.hpp"
#include "tank/pipe_and_wire.hpp"
#include "detection/radar.hpp"

class small : public ship_hull{
public:
  small(std::string name, plogger logger);
  vector_2d force(timestamp const &time) override;

protected:
  struct {
    std::shared_ptr<reactor> master_core_, slave_core_;
    pfuel_tank_line reactor_fuel_;
    pbattery_line main_battery_;
    pbattery_line apu_;
  } power_system_;

  virtual void construct_power_system();

  struct {
    std::shared_ptr<engine> main_engine_, slave_engine_[2];
    pfuel_tank_line main_fuel_;
    pfuel_tank_line slave_fuel_lines_[2];
  } engine_system_;

  virtual void construct_engine_system();

  using radar = long_range_radar<radio_unit, radio_transmitter_option>;

  std::shared_ptr<radar> radar_;
};

#endif // DSCS_SMALL_HPP
