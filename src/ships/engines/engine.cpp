//
// Created by yalavrinenko on 21.08.2019.
//

#include "engine.hpp"
void engine::action() {
  if (align_angle() != 0)
    align_engine();

  if (engine_state::active == current_state_){
    current_thrust_ += option_.ignition_speed;
    if (current_thrust_ > option_.max_thrust)
      current_thrust_ = option_.max_thrust;

    auto required_fuel = current_thrust_ * option_.fuel_consumption_per_thrust;
    auto fuel = option_.inputs.fuel_supply.pull(required_fuel);

    current_fuel_comsumption_ += fuel;

    auto required_charde = fuel * option_.power_consumption_per_fuel;
    auto charge = option_.inputs.energy_supply.pull(required_charde);

    current_power_consumption_ += charge;

    current_thrust_ = fuel * (1.0 / option_.fuel_consumption_per_thrust) * (charge / required_charde);

    if (std::isnan(current_thrust_))
      current_thrust_ = 0;

  } else {
    current_thrust_ = 0;
  }
}

void engine::log_action() const {
  logger()->log("Engine [", name(),"]:");
  logger()->log("\tState:", (current_state_ == engine_state::active) ? "Active" : "Disable");
  logger()->log("\tFuel consumption:", current_fuel_comsumption_);
  logger()->log("\tPower consumption:", current_power_consumption_);
  logger()->log("\tCurrent thrust_level:", current_thrust_);
  logger()->log("\tAlign angle:", align_angle());
  logger()->log("\tThrust direction (x, y):", current_thrust_direction_.x,";", current_thrust_direction_.y);
  current_fuel_comsumption_ = current_power_consumption_ = 0;
}

double engine::mass() const { return icomponent::mass(); }

engine::engine(double mass, engine_option const &option,
                 std::string const &name, plogger logger)
    : icomponent(mass, "Engine " + name, std::move(logger), component_type::engine), option_(option) {}

void engine::align_engine() {
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
