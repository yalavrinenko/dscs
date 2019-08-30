//
// Created by yalavrinenko on 24.08.2019.
//

#include "../src/ships/small.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>

TEST(ship, exist){
  logger_factory factory("../../ramfs");

  small small_ship("Ship S1", factory.create_logger("S1", 1024*10));

  timestamp ts;
  for (auto i = 0; i < 10000; ++i){
    ++ts;
    small_ship.update(ts);
    small_ship.log_action();
    factory.flush_loggers();
  }
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}