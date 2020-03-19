//
// Created by yalavrinenko on 06.11.2019.
//
//
// Created by yalavrinenko on 10.09.2019.
//
#include "../src/drifting_hulk.hpp"
#include "../src/ships/control/actions.hpp"
#include "../src/ships/control/control_units/time_controller.hpp"
#include "../src/ships/small.hpp"
#include "../src/void_map.hpp"
#include "../src/ships/weapons/missile/controlled_missile.hpp"
#include <gtest/gtest.h>
#include <random>
#include <thread>

class Random{
public:
  double uniform(double a, double b){
    std::uniform_real_distribution<double> r(a, b);
    return r(engine);
  }

  vector_2d uniform_v(double a, double b){
    return {uniform(a, b), uniform(a, b)};
  }
private:
  std::mt19937_64 engine{std::random_device{}()};
};

TEST(missile, real_space){
  auto factory_ptr = std::make_shared<logger_factory>("../../ramfs");

  void_map space(factory_ptr);

  Random r;

  auto small_ship = std::make_unique<small>("Ship_S", factory_ptr->create_logger("S1.ship", 1024*10));
  small_ship->add_control_unit(std::make_unique<timed_control>());
  space.add_object(std::move(small_ship), r.uniform_v(-1, 1), r.uniform_v(-0.01, 0.01));

  int N = 10;
  for (auto i = 0; i < N; ++i){
    auto hulk = std::make_unique<drifting_hulk>(r.uniform(1000, 10000));
    space.add_object(std::move(hulk),
                     r.uniform_v(-1000, 1000),
                     r.uniform_v(-0.01, 0.01));
  }

  auto m_1 = std::make_unique<missile>(412.0, 1, "Missile_1", factory_ptr->create_logger("Missile_1.missile"));

  auto tank = resource_line_factory<fuel_tank_line>::construct_line(1, 100, 100, "", nullptr, 4, 5000, 1, 1);
  while (tank->push(100));

  auto battery = resource_line_factory<battery_line>::construct_line(1, 50, 50, "", nullptr, 4, 5000, 1);
  while (battery->push(100));

  fuel_pipe p(tank, battery, 0.01);
  wire w(battery);

  bool is_charged = false, is_refueled = false;
  while (!is_charged || !is_refueled){
    if (!is_charged)
      is_charged = m_1->charge(w);
    if (!is_refueled)
      is_refueled = m_1->refule(p);
  }

  space.add_object(std::move(m_1), {0, 0}, {0, 0});

  std::thread t([&space](){
    std::this_thread::sleep_for(1200s);
    space.stop();
  });

  space.run();

  t.join();
}

TEST(missile, create){
  auto factory_ptr = std::make_shared<logger_factory>("../../ramfs");

  void_map space(factory_ptr);

  auto m_1 = std::make_unique<missile>(412.0, 1, "Missile_1", factory_ptr->create_logger("Missile_1.missile"));

  auto tank = resource_line_factory<fuel_tank_line>::construct_line(1, 100, 100, "", nullptr, 4, 5000, 1, 1);
  while (tank->push(100));

  auto battery = resource_line_factory<battery_line>::construct_line(1, 50, 50, "", nullptr, 4, 5000, 1);
  while (battery->push(100));

  fuel_pipe p(tank, battery, 0.01);
  wire w(battery);

  bool is_charged = false, is_refueled = false;
  while (!is_charged || !is_refueled){
    if (!is_charged)
      is_charged = m_1->charge(w);
    if (!is_refueled)
      is_refueled = m_1->refule(p);
  }

  space.add_object(std::move(m_1), {0, 0}, {0, 0});

  Random r;
  int N = 10;
  for (auto i = 0; i < N; ++i){
    auto hulk = std::make_unique<drifting_hulk>(r.uniform(1000, 10000));
    space.add_object(std::move(hulk),
                     r.uniform_v(-1000, 1000),
                     r.uniform_v(-0.01, 0.01));
  }

  std::thread t([&space](){
    std::this_thread::sleep_for(1200s);
    space.stop();
  });

  space.run();

  t.join();
}

int main(int argc, char** argv){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}