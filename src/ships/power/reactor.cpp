//
// Created by yalavrinenko on 21.08.2019.
//
#include "reactor.hpp"
#include <utils/gui_input.hpp>
#include <utility>

void reactor::ignite() {
  if (state_ == reactor_state::shutdown) {
    state_ = reactor_state::ignition;
    current_power_ = 0;

    tmp_power_line_ = option_.nuclear_fuel.electric_line();
    option_.nuclear_fuel.set_electric_line(apu_);
  }
}

void reactor::ignite_action() {
  auto power_ = current_power_ + option_.ignition_speed;

  auto required_fuel = power_ * option_.fuel_per_charge;
  auto fuel = option_.nuclear_fuel.pull(required_fuel);

  fuel_consumtion_ = fuel;

  auto required_charge = fuel * option_.ignition_charge_consumption;
  auto charge = apu_->pull(required_charge);

  charge_consumption_ = charge;
  current_power_ = fuel * (1.0 / option_.fuel_per_charge) * (charge / required_charge);

  if (charge == 0)
    state_ = reactor_state::shutdown;

  if (current_power_ > option_.peak_output) {
    option_.nuclear_fuel.set_electric_line(tmp_power_line_);
    option_.output_wire.push(current_power_);
    next_power_ = option_.peak_output;
    state_ = reactor_state::active;
  }
}

void reactor::log_action() const {
  logger()->log("Reactor [", name(), "]:");
  logger()->log("\tMass:", mass());
  logger()->log("\tState:", std::to_string(state_));
  logger()->log("\tOutput power:", current_power_);
  logger()->log("\tFuel consumption:", fuel_consumtion_);
  logger()->log("\tElectric consumption:", charge_consumption_);

  logger()->up_level();
  for (auto &component : components_)
    component->log_action();
  logger()->down_level();
}

reactor::reactor(double mass, reactor_option option,
                 std::string const &name, pbattery_line apu,
                 plogger logger) : icomponent(mass, "Reactor " + name, std::move(logger), component_type::reactor), option_(std::move(option)),
                                   initial_max_output_power_{option_.peak_output},
                                   apu_{std::move(apu)} {
  //components_.push_back(apu_);

  using namespace gui;
  guis_.text = this->add_gui_entry<text_entry>(this->name());
  guis_.num = this->add_gui_entry<numeric_entry>(this->name());

  register_control_action();
}

void reactor::active_action() {
  auto occupancy = option_.output_wire.occupancy();

  next_power_ += (next_power_ < option_.peak_output) ? option_.ignition_speed : -option_.ignition_speed;

  if (next_power_ < 0.1 * option_.peak_output)
    next_power_ = 0.1 * option_.peak_output;

  auto power = next_power_ * ((occupancy > 0.9) ? 1.0 - ((occupancy - 0.9) / 0.1) * 0.999 : 1);

  auto required_fuel = power * option_.fuel_per_charge;
  auto fuel = option_.nuclear_fuel.pull(required_fuel);
  fuel_consumtion_ = fuel;

  if (fuel == 0) {
    current_power_ = 0;
    state_ = reactor_state::shutdown;
  }

  auto required_charge = fuel * option_.ignition_charge_consumption;
  auto charge = option_.output_wire.pull(required_charge);
  charge_consumption_ = charge;

  current_power_ = fuel * (1.0 / option_.fuel_per_charge) * (charge / required_charge);

  auto apu_power = apu_->push(current_power_);
  option_.output_wire.push(current_power_ - apu_power);
}

void reactor::draw() {
  guis_.text->log("Mass:\t", std::to_string(mass()));
  guis_.text->log("State:", std::to_string(state_));

  guis_.num->log("Output power:", current_power_, this->option_.peak_output);
  guis_.num->log("Fuel consumption:", fuel_consumtion_);
  guis_.num->log("Electric consumption:", charge_consumption_);

  for (auto &component : components_)
    component->draw();
}
void reactor::register_control_action() {
  auto controls = this->add_gui_entry<gui::gui_controls>(this->name());

  auto ignite_action = [this](gui::icontrol &obj) { this->ignite(); };
  auto shutdown_action = [this](gui::icontrol &obj) { this->shutdown(); };

  auto button_group = std::make_unique<gui::button_group<gui::button_control, 2>>(
      std::initializer_list<gui::button_control>{{"Ignite", ignite_action}, {"Shutdown", shutdown_action}});
  controls->add_control(std::move(button_group));
}

reactor::~reactor() = default;

std::string std::to_string(reactor_state const &state) {
  if (state == reactor_state::active)
    return "active"s;
  if (state == reactor_state::ignition)
    return "ignition"s;
  if (state == reactor_state::shutdown)
    return "shutdown"s;
  return "unknown"s;
}
