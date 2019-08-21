//
// Created by yalavrinenko on 21.08.2019.
//
#include <gtest/gtest.h>
#include "../src/ships/tank/fuel_tank.hpp"
#include "../src/ships/tank/battery.hpp"
#include <thread>
#include <random>

TEST(fuel, create){
  logger_factory factory("../../ramfs");
  auto logger = factory.create_logger("fuel_tank");
  auto tank = fuel_tank(10, 1.0, 1.0, logger);
  tank.log_action();

  factory.flush_loggers();

  std::cout << std::endl;
  system(("cat "s + logger->linked_path().string()).c_str());
}

TEST(fuel, lifeflow){
  logger_factory factory("../../ramfs");
  auto logger = factory.create_logger("fuel_tank");
  auto tank = fuel_tank(10, 1.0, 1.0, logger);

  auto log_check = [&logger, &tank, &factory](){
    tank.log_action();
    factory.flush_loggers();
    std::cout << std::endl;
    system(("cat "s + logger->linked_path().string()).c_str());
  };

  log_check();
  tank.push(10);
  log_check();

  for (auto i = 0; i < 20; ++i){
    tank.push(0.5);
    log_check();
  }

  for (auto i = 0; i < 30; ++i){
    std::cout << tank.pull(0.5);
    log_check();
  }
}

TEST(fuel, random_flow){
  logger_factory factory("../../ramfs");
  auto logger = factory.create_logger("fuel_tank");
  auto tank = fuel_tank(10, 1.0, 1.0, logger);

  tank.push(10);

  std::mt19937_64 engine{std::random_device{}()};
  std::uniform_real_distribution<double> uniform{-1, 1};
  for (auto i = 0; i < 200000; ++i){
    auto q = uniform(engine);
    if (q < 0)
      tank.pull(-q);
    else
      tank.push(q);

    tank.log_action();
    factory.flush_loggers();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

TEST(fuel, with_battery){
  logger_factory factory("../../ramfs");
  auto tank = fuel_tank(10, 1.0, 1.0, factory.create_logger("fuel_tank"));
  auto charge_el = battery(20.0, 10.0, factory.create_logger("battery"));

  tank.push(10);
  charge_el.push(20.0);

  std::mt19937_64 engine{std::random_device{}()};
  std::uniform_real_distribution<double> uniform{-1, 1};
  for (auto i = 0; i < 200000; ++i){
    auto manipulate = [&engine, &uniform](auto &container){
      auto q = uniform(engine);
      if (q < 0)
        container.pull(-q);
      else
        container.push(q);
      container.log_action();
    };

    manipulate(tank);
    manipulate(charge_el);

    factory.flush_loggers();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}