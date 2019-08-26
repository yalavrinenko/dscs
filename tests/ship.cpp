//
// Created by yalavrinenko on 24.08.2019.
//

#include "../src/ships/tank/battery_adapter.hpp"
#include "../src/ships/tank/fuel_adapter.hpp"
#include "../src/ships/tank/pipe_and_wire.hpp"
#include "../src/ships/power/reactor.hpp"
#include "../src/ships/engines/engine.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>


int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}