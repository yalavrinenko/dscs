//
// Created by yalavrinenko on 21.08.2019.
//

#include "../src/ships/tank/battery_adapter.hpp"
#include "../src/ships/tank/fuel_adapter.hpp"
#include "../src/ships/tank/pipe_and_wire.hpp"
#include "../src/ships/power/reactor.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>

TEST(adapters, evolution_with_wire){
  logger_factory factory("../../ramfs");
  auto fuel_line = std::make_shared<fuel_tank_line>(100, 2.0, 5.0, "General line", factory.create_logger("Fuel line"));

  fuel_line->add_tank(200, 1.0, 2.0);
  fuel_line->add_tank(200, 1.0, 2.0);

  while (fuel_line->push(1.0) > 0);

  auto general_battery_line = std::make_shared<battery_line>(100, 2.0, 5.0, "General line", factory.create_logger("Battery line"));
  general_battery_line->add_tank(1000, 5.0);

  wire electric_wire(general_battery_line);
  fuel_pipe pipe(fuel_line, general_battery_line, 0.1);

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

  auto  power_unit = std::make_shared<reactor>(1000.0, r_option, "Main power unit"s, factory.create_logger("Reactor"));
  auto  power_unit_1 = std::make_shared<reactor>(1000.0, r_option, "Main power unit"s, factory.create_logger("Reactor 1"));
  auto  power_unit_2 = std::make_shared<reactor>(1000.0, r_option, "Main power unit"s, factory.create_logger("Reactor 2"));

  for (auto i = 0; i < 200000; ++i){
    if (i == 10)
      power_unit->ignite();

    if (i == 20)
      power_unit_1->ignite();

    if (i == 30)
      power_unit_2->ignite();

    auto manipulate = [](auto &container){
      container->action();
      container->log_action();
    };

    manipulate(power_unit);
    manipulate(power_unit_1);
    manipulate(power_unit_2);
    manipulate(fuel_line);
    manipulate(general_battery_line);

    factory.flush_loggers();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}


int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}