//
// Created by yalavrinenko on 21.08.2019.
//

#include "engine.hpp"
void iengine::action() {
  if (align_angle() != 0)
    align_engine();
}
void iengine::log_action() const {
  logger()->log("Engine [", name(),"]:");
  logger()->log("\tFuel consumption:", current_fuel_comsumption_);
  logger()->log("\tPower consumption:", current_power_consumption_);
  logger()->log("\tCurrent thrust:", current_thrust_);
  logger()->log("\tAlign angle:", align_angle());
  logger()->log("\tThrust direction (x, y):", current_thrust_direction_.x,";", current_thrust_direction_.y);
  current_fuel_comsumption_ = current_power_consumption_ = 0;
}

double iengine::mass() const { return icomponent::mass(); }

iengine::iengine(double mass, engine_option const &option,
                 std::string const &name, plogger logger)
    : icomponent(mass, "Engine " + name, std::move(logger)), option_(option) {}

void iengine::align_engine() {
  vector_2d delta {option_.thrust_align_speed, option_.thrust_align_speed};
  auto sign = [](auto v){
    return (v < 0) ? -1: 1;
  };

  vector_2d align_distance = target_thrust_direction_ - current_thrust_direction_;
  vector_2d align_sign {static_cast<double>(sign(align_distance.x)), static_cast<double>(sign(align_distance.y))};

  delta = delta * align_sign;
  if (std::fabs(delta.x) > std::fabs(align_distance.x))
    delta.x = align_sign.x * std::fabs(align_distance.x);

  if (std::fabs(delta.y) > std::fabs(align_distance.y))
    delta.y = align_sign.y * std::fabs(align_distance.y);

  auto required_charge = (delta * option_.align_charge).len();
  auto charge = option_.inputs.energy_supply.pull(required_charge);
  if (required_charge > 0)
    delta = delta * charge / required_charge;

  current_power_consumption_ += charge;
  current_thrust_direction_ = current_thrust_direction_ + delta;
}
