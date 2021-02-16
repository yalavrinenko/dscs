//
// Created by yalavrinenko on 16.02.2021.
//

#include "missile.hpp"
#include "../../../component-factrory.hpp"
#include "../../engines/engine.hpp"
#include "../../tank/battery.hpp"
#include "../../tank/fuel_tank.hpp"
#include <ships/weapons/warhead.hpp>

vector_2d missile::force(timestamp const &time) { return engine_->thrust(); }

bool missile::refule(fuel_pipe &source) {
  double get = source.pull(fuel_->input_power());

  auto pushed = fuel_->push(get);
  source.push(get - pushed);

  return pushed == 0;
}

bool missile::charge(wire &source) {
  double charge = source.pull(battery_->input_power());

  auto pushed = battery_->push(charge);
  source.push(charge - pushed);

  return pushed == 0;
}

void missile::setup_warhead(std::shared_ptr<warhead> warhead) {
  if (warhead_ != nullptr)
    throw std::logic_error("Double warhead not supported");

  warhead_ = std::move(warhead);
  this->add_component(warhead_, true);
}

missile::missile(std::string name, plogger logger)
    : ship_hull(0.25, std::move(name), std::move(logger)){

  battery_ = resource_line_factory<battery_line>::construct_line(0.01, 10, 50, "Battery", this->slogger(),
                                                                 6,
                                                                 battery_config<component_size::tiny>::capacity, battery_config<component_size::tiny>::mass);

  fuel_ = resource_line_factory<fuel_tank_line>::construct_line(0.5, 10, 50, "Fuel", this->slogger(), 20,
                                                                fuel_tank_config<component_size::tiny, fuel_type::liquid>::capacity,
                                                                fuel_tank_config<component_size::tiny, fuel_type::liquid>::mass,
                                                                fuel_tank_config<component_size::tiny, fuel_type::liquid>::fuel_mass);

  wire power_supply(battery_);
  fuel_pipe main_pipe(fuel_, battery_, 0.05);
  engine_ = EngineFactory::construct<component_size::tiny>(main_pipe, power_supply, "Engine", this->slogger());

  add_component(battery_);
  add_component(fuel_);
  add_component(engine_, true);
}
