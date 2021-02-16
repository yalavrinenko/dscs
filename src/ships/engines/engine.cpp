//
// Created by yalavrinenko on 21.08.2019.
//

#include "engine.hpp"
#include <utils/gui_input.hpp>
void engine::action() {
  current_fuel_comsumption_ = current_power_consumption_ = 0;

  if (align_angle() != 0)
    align_engine();

  if (engine_state::active == current_state_) {
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
  logger()->log("Engine [", name(), "]:");
  logger()->log("\tState:", (current_state_ == engine_state::active) ? "Active" : "Disable");
  logger()->log("\tFuel consumption:", current_fuel_comsumption_);
  logger()->log("\tPower consumption:", current_power_consumption_);
  logger()->log("\tCurrent thrust_level:", current_thrust_);
  logger()->log("\tAlign angle:", align_angle());
  logger()->log("\tThrust direction (x, y):", current_thrust_direction_.x, ";", current_thrust_direction_.y);
  current_fuel_comsumption_ = current_power_consumption_ = 0;
}

double engine::mass() const { return icomponent::mass(); }

engine::engine(double mass, engine_option const &option,
               std::string const &name, plogger logger)
    : icomponent(mass, "Engine " + name, std::move(logger), component_type::engine), option_(option) {
  add_gui_entry<gui::text_entry>(this->name());
  add_gui_entry<gui::numeric_entry>(this->name());
  add_gui_entry<gui::moving_plot_entry>(this->name(), 100);
  add_gui_entry<gui::polar_entry>(this->name(), 1.0);

  if (this->slogger().gui_window != nullptr)
    register_control_action();
}

void engine::align_engine() {
  vector_2d delta{option_.thrust_align_speed, option_.thrust_align_speed};
  auto sign = [](auto v) {
    return (v < 0) ? -1 : 1;
  };

  vector_2d align_distance = target_thrust_direction_ - current_thrust_direction_;
  vector_2d align_sign{static_cast<double>(sign(align_distance.x)), static_cast<double>(sign(align_distance.y))};

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

void engine::draw() {
  auto ltext = entry<gui::text_entry>();
  if (ltext)
    ltext->log("State:", (current_state_ == engine_state::active) ? "Active" : "Disable");

  auto lplot = entry<gui::moving_plot_entry>();
  if (lplot) {
    lplot->log("Fuel consumption:", current_fuel_comsumption_);
    lplot->log("Power consumption:", current_power_consumption_);
  }

  auto nplot = entry<gui::numeric_entry>();
  if (nplot)
    nplot->log("Current thrust_level:", current_thrust_, thrust_peak_limit_);


  std::stringstream ss;

  auto thrust_direction = [this](int i) -> std::pair<double, double> {
    if (i == -1) return {1, 1};
    return {current_thrust_direction_.in_polar().x, current_thrust_direction_.in_polar().y};
  };

  ss << "Target align angle:\t" << align_angle() << "\n"
     << "Polar angle (r, phi):\t"
     << "(" << thrust_direction(0).first << ", " << thrust_direction(0).second << ")";

  auto pplot = entry<gui::polar_entry>();
  pplot->log(ss.str(), thrust_direction);
}

void engine::register_control_action() {
  auto controls = this->add_gui_entry<gui::gui_controls>(this->name());

  auto align_action = [this](auto &obj) {
    auto const& angle_control = dynamic_cast<gui::angle_control const&>(obj);
    this->align(angle_control.angle());
  };

  auto align_angle = [this](auto &obj) {
    return this->align_angle();
  };

  controls->add_control<gui::angle_control>("Rotation speed", align_action, align_angle, std::make_pair(-50.0, 50.0));

  auto activate = [this](auto &obj) { this->ignite(); };
  auto shutdown = [this](auto &obj) { this->shutdown(); };

  auto button_group = std::make_unique<gui::button_group<gui::button_control, 2>>(
      std::initializer_list < gui::button_control>{
          {"Ignite", activate},
          {"Shutdown", shutdown}});
  controls->add_control(std::move(button_group));

  auto thurst_setter_ = [this](auto &obj) { return this->option_.max_thrust / thrust_peak_limit_ * 100.0; };
  auto change_thrust = [this](auto &obj){
    auto const& slider = dynamic_cast<gui::slider_control const &>(obj);
    this->set_max_thrust(slider.value() * thrust_peak_limit_ / 100.0);
  };
  controls->add_control<gui::slider_control>("Max thrust", change_thrust, thurst_setter_, std::make_pair(10, 100));
}
