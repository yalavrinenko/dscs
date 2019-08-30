//
// Created by yalavrinenko on 24.08.2019.
//

#include "../src/ships/small.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>
#include "../src/ships/control/actions.hpp"
#include "../src/ships/control/control_units/time_controller.hpp"

TEST(ship, exist){
  logger_factory factory("../../ramfs");

  small small_ship("Ship S1", factory.create_logger("S1", 1024*10));
  small_ship.add_control_unit(std::make_unique<timed_control>());
  action_queue actions;
  timestamp ts;
  for (auto i = 0; i < 10000; ++i){
    ++ts;
    while (actions.invoke_next(ts));

    small_ship.update(ts);
    small_ship.log_action();

    actions.push_actions(small_ship.extract_control_actions(ts));
    factory.flush_loggers();
  }
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}