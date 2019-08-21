//
// Created by yalavrinenko on 21.08.2019.
//

#include "../src/ships/tank/battery_adapter.hpp"
#include "../src/ships/tank/fuel_adapter.hpp"
#include "../src/ships/tank/pipe_and_wire.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>

TEST(adapter, fuel_adapter){
  logger_factory factory("../../ramfs");
  auto fuel_line = fuel_tank_line(100, 2.0, 5.0, "General line", factory.create_logger("Fuel line"));
  ASSERT_EQ(fuel_line.mass(), 100);

  fuel_line.add_tank(10, 1, 1.0);
  fuel_line.add_tank(20, 2, 1.0);
  ASSERT_EQ(fuel_line.mass(), 100 + 1 + 2);

  auto i = 15;
  while (i-- > 0)
    fuel_line.push(2);
  ASSERT_EQ(fuel_line.mass(), 100 + 1 + 2 + 30);

  fuel_line.push(30);
  ASSERT_EQ(fuel_line.mass(), 100 + 1 + 2 + 30);

  i = 10;
  while (i-- > 0)
    fuel_line.pull(2);
  ASSERT_EQ(fuel_line.mass(), 100 + 1 + 2 + 30 - 20);
}

TEST(adapter, battery_adapter){
  logger_factory factory("../../ramfs");
  auto general_battery_line = battery_line(100, 2.0, 5.0, "General line", factory.create_logger("Battery line"));
  ASSERT_EQ(general_battery_line.mass(), 100);

  general_battery_line.add_tank(10, 1);
  general_battery_line.add_tank(20, 2);
  ASSERT_EQ(general_battery_line.mass(), 100 + 1 + 2);

  auto i = 15;
  while (i-- > 0)
    general_battery_line.push(2);
  ASSERT_EQ(general_battery_line.mass(), 100 + 1 + 2);

  general_battery_line.push(30);
  ASSERT_EQ(general_battery_line.mass(), 100 + 1 + 2);

  i = 10;
  while (i-- > 0)
    general_battery_line.pull(2);
  ASSERT_EQ(general_battery_line.mass(), 100 + 1 + 2);
}

TEST(adapters, evolution){
  logger_factory factory("../../ramfs");
  auto fuel_line = fuel_tank_line(100, 2.0, 5.0, "General line", factory.create_logger("Fuel line"));

  fuel_line.add_tank(10, 1.0, 2.0);
  fuel_line.add_tank(10, 1.0, 2.0);
  fuel_line.add_tank(10, 1.0, 2.0);
  fuel_line.add_tank(10, 1.0, 2.0);
  fuel_line.add_tank(10, 1.0, 2.0);

  auto general_battery_line = battery_line(100, 2.0, 5.0, "General line", factory.create_logger("Battery line"));
  general_battery_line.add_tank(100, 5.0);
  general_battery_line.add_tank(100, 5.0);
  general_battery_line.add_tank(100, 5.0);

  std::mt19937_64 engine{std::random_device{}()};
  std::uniform_real_distribution<double> uniform{-5, 10};
  for (auto i = 0; i < 200000; ++i){
    auto manipulate = [&engine, &uniform](auto &container){
      auto q = uniform(engine);
      container.action();
      if (q < 0)
        container.pull(-q);
      else
        container.push(q);
      container.log_action();
    };

    manipulate(fuel_line);
    manipulate(general_battery_line);

    factory.flush_loggers();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

TEST(adapters, evolution_with_wire){
  logger_factory factory("../../ramfs");
  auto fuel_line = std::make_shared<fuel_tank_line>(100, 2.0, 5.0, "General line", factory.create_logger("Fuel line"));

  fuel_line->add_tank(10, 1.0, 2.0);
  fuel_line->add_tank(10, 1.0, 2.0);
  fuel_line->add_tank(10, 1.0, 2.0);
  fuel_line->add_tank(10, 1.0, 2.0);
  fuel_line->add_tank(10, 1.0, 2.0);

  auto general_battery_line = std::make_shared<battery_line>(100, 2.0, 5.0, "General line", factory.create_logger("Battery line"));
  general_battery_line->add_tank(100, 5.0);
  general_battery_line->add_tank(100, 5.0);
  general_battery_line->add_tank(100, 5.0);

  wire electric_wire(general_battery_line);
  fuel_pipe pipe(fuel_line, general_battery_line, 0.1);

  std::mt19937_64 engine{std::random_device{}()};
  std::uniform_real_distribution<double> uniform{-5, 10};
  for (auto i = 0; i < 200000; ++i){
    auto manipulate = [&engine, &uniform](auto &container, auto &pipe){
      auto q = uniform(engine);

      container->action();
      if (q < 0)
        pipe.pull(-q);
      else
        pipe.push(q);

      container->log_action();
    };

    manipulate(fuel_line, pipe);
    manipulate(general_battery_line, electric_wire);

    factory.flush_loggers();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}