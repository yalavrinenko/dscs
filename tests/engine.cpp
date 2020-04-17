//
// Created by yalavrinenko on 21.08.2019.
//
#include "../src/ships/tank/battery_adapter.hpp"
#include "../src/ships/tank/fuel_adapter.hpp"
#include "../src/ships/tank/pipe_and_wire.hpp"
#include "../src/ships/power/reactor.hpp"
#include "../src/ships/engines/engine.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>

TEST(adapters, evolution_with_wire){
  auto gui = gui::logger_window::create("Ship1");

  auto fuel_line = std::make_shared<fuel_tank_line>(100, 2.0, 5.0, "Reactor fuel", nullptr);
  fuel_line->add_tank(200, 1.0, 2.0);
  fuel_line->add_tank(200, 1.0, 2.0);
  fuel_line->add_tank(200, 1.0, 2.0);
  while (fuel_line->push(1.0) > 0);

  auto engine_fuel = std::make_shared<fuel_tank_line>(100, 2.0, 10.0, "Engine fuel", nullptr);
  engine_fuel->add_tank(2000, 1.0, 2.0);
  while (engine_fuel->push(2.0) > 0);

  auto general_battery_line = std::make_shared<battery_line>(100, 2.0, 10.0, "Power line", nullptr);
  general_battery_line->add_tank(1000, 5.0);

  wire electric_wire(general_battery_line);
  fuel_pipe pipe(fuel_line, general_battery_line, 0.1);
  fuel_pipe engine_pipe(engine_fuel, general_battery_line, 1.0);

  reactor_option r_option{
      {
          10.0,
          100.0,
          1.0,
          5.0
      },
      pipe,
      electric_wire,
      2.0,
      0.1,
      0.01,
      1.0
  };

  auto  power_unit = std::make_shared<reactor>(1000.0, r_option, "Reactor Main "s, nullptr);

  engine_option e_option{
    10.0,
    0.1,
    {electric_wire, engine_pipe},
    1.0,
    2.0,
    0.01,
    1.0
  };

  auto eng = std::make_shared<engine>(10000, e_option, "Initial engine", nullptr);

  std::vector<pscomponent> components{fuel_line, engine_fuel, general_battery_line, power_unit, eng};

  auto is_ignite = false;
  for (auto i = 0; i < 200000; ++i){
    if (i == 10)
      power_unit->ignite();

    if (i == 0)
      eng->align({-1, -1});

    if (!std::isnan(eng->align_angle()) && eng->align_angle() < 1.0 && !is_ignite && i > 500 && general_battery_line->percent_load() > 0.9) {
      eng->ignite();
      is_ignite = true;
    }

    if (general_battery_line->percent_load() < 0.1) {
      eng->shutdown();
      is_ignite = false;
    }

    if (i > 500 && power_unit->state() == reactor_state::shutdown)
      power_unit->ignite();

    for (auto &component: components)
      component->action();

    for (auto &component: components)
      component->draw();

    gui->flush();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}


int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}