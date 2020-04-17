//
// Created by yalavrinenko on 07.04.19.
//

#include "small.hpp"
#include "../component-factrory.hpp"
small::small(std::string name, plogger logger):
  ship_hull(1e+6, std::move(name), std::move(logger)){

  construct_power_system();
  construct_engine_system();

  wire radar_wire(power_system_.main_battery_);
  radio_transmitter_option r_option{radar_wire};

  radar_ = std::make_unique<radar>(
      radio_unit_config<component_size::medium>::mass, 5, r_option, this->env_.EM_Field(), "Main Radar",
      this->slogger());

  add_component(radar_, true);
}

void small::construct_power_system() {
  power_system_.reactor_fuel_ = std::make_shared<fuel_tank_line>(
      1.0, 100, 10, "Main reactor fuel line", slogger());

  unsigned const FUEL_BANK_COUNT = 5;
  double const FUEL_BACK_SIZE = 5000;
  for (auto i = 0; i < FUEL_BANK_COUNT; ++i)
    power_system_.reactor_fuel_->add_tank(FUEL_BACK_SIZE, 0.1, 0.1);
  while (power_system_.reactor_fuel_->push(100.0) > 0);

  unsigned const POWER_BANK_COUNT = 25;

  power_system_.main_battery_ = resource_line_factory<battery_line>::construct_line(
      0.2, 100, 100, "Main power line", slogger(), POWER_BANK_COUNT,
      battery_config<component_size::medium>::capacity,
      battery_config<component_size::medium>::mass);

  fuel_pipe reactor_pipe(power_system_.reactor_fuel_, power_system_.main_battery_, 0.1);
  wire reactor_out_wire(power_system_.main_battery_);

  power_system_.apu_ = std::make_shared<battery_line>(0.1, 0.5, 5.0, "APU Line"s, this->slogger());
  power_system_.apu_->add_tank(60, 1.0);
  while (power_system_.apu_->push(0.5) > 0);

  power_system_.master_core_ = ReactorFactory::construct<component_size::small>(reactor_pipe, reactor_out_wire, "Master PU", power_system_.apu_, slogger());
  power_system_.slave_core_ = ReactorFactory::construct<component_size::tiny>(reactor_pipe, reactor_out_wire, "Slave PU", power_system_.apu_, slogger());

  add_component(power_system_.master_core_, true);
  add_component(power_system_.slave_core_, true);
  add_component(power_system_.reactor_fuel_);
  add_component(power_system_.main_battery_);
  add_component(power_system_.apu_);
}
void small::construct_engine_system() {
  auto make_fuel_line = [this](auto banks, auto volume, auto name){
    auto fuel_line = std::make_shared<fuel_tank_line>(1.0, 100, 100, std::move(name), slogger());
    for (auto i = 0u; i < banks; ++i)
      fuel_line->add_tank(volume, 0.1, 1);
    while (fuel_line->push(100.0) > 0);
    return fuel_line;
  };

  unsigned const MAIN_FUEL_BANKS = 100;
  double const MAIN_FUEL_VOLUME = 5000.0;
  engine_system_.main_fuel_ = std::move(make_fuel_line(MAIN_FUEL_BANKS, MAIN_FUEL_VOLUME, "Main fuel line"));

  unsigned const SLAVE_FUEL_BANKS = 20;
  double const SLAVE_FUEL_VOLUME = 1000.0;
  for (auto i = 0; i < 2; ++i)
    engine_system_.slave_fuel_lines_[i] = std::move(make_fuel_line(SLAVE_FUEL_BANKS, SLAVE_FUEL_VOLUME, "Slave fuel line " + std::to_string(i)));

  wire power_supply(power_system_.main_battery_);
  fuel_pipe main_pipe(engine_system_.main_fuel_, power_system_.main_battery_, 0.1);
  engine_system_.main_engine_ = EngineFactory::construct<component_size::small>(main_pipe, power_supply, "Master Engine", slogger());

  add_component(engine_system_.main_engine_, true);
  add_component(engine_system_.main_fuel_);

  for (auto i = 0; i < 2; ++i){
    fuel_pipe slave_pipe(engine_system_.slave_fuel_lines_[i], power_system_.main_battery_, 0.1);
    engine_system_.slave_engine_[i] = EngineFactory::construct<component_size::tiny>(slave_pipe, power_supply, "Slave Engine " + std::to_string((i)), slogger());

    add_component(engine_system_.slave_engine_[i], true);
    add_component(engine_system_.slave_fuel_lines_[i]);
  }
}

vector_2d small::force(timestamp const &time) {
  return engine_system_.main_engine_->thrust() +
    engine_system_.slave_engine_[0]->thrust() +
    engine_system_.slave_engine_[1]->thrust();
}
