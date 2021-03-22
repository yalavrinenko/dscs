//
// Created by yalavrinenko on 03.04.19.
//
#include "src/drifting_hulk.hpp"
#include "src/ships/control/control_units/time_controller.hpp"
#include "src/ships/small.hpp"
#include "src/void_map.hpp"
#include <future>
#include <random>
#include <ships/weapons/missiles/NAR_M.hpp>
#include <ships/weapons/warhead.hpp>

class Random {
public:
  double uniform(double a, double b) {
    std::uniform_real_distribution<double> r(a, b);
    return r(engine);
  }

  vector_2d uniform_v(double a, double b) { return {uniform(a, b), uniform(a, b)}; }

private:
  std::mt19937_64 engine{std::random_device{}()};
};

using namespace std::chrono_literals;

int main(int argc, char **argv) {
  auto factory_ptr = std::make_shared<logger_factory>("../ramfs");
  auto gui_factory = gui::logger_environment::create();

  void_map space(factory_ptr, gui_factory);

  Random r;
  [[maybe_unused]] int N = 10;

  //for (auto i = 0; i < N; ++i) {

    //space.add_object(std::move(hulk), r.uniform_v(-300, 300), r.uniform_v(-100, 100));
  space.add_object(std::make_unique<drifting_hulk>(r.uniform(1000, 10000)), {100, 100}, {-20, -20});
  space.add_object(std::make_unique<drifting_hulk>(r.uniform(1000, 10000)), {-100, 100}, {20, -20});
  space.add_object(std::make_unique<drifting_hulk>(r.uniform(1000, 10000)), {100, -100}, {-20, 20});
  space.add_object(std::make_unique<drifting_hulk>(r.uniform(1000, 10000)), {-100, -100}, {20, 20});
  //}

  auto gui_ship = gui_factory->create_logger("Ship_S");
  auto text_log = factory_ptr->create_logger("S1.ship", 1024 * 10);
  auto small_ship = std::make_unique<small>("Ship_S", plogger{text_log, gui_ship});
  small_ship->equip();

  for (std::weakly_incrementable auto i: std::views::iota(0, 20)) {
    std::unique_ptr<icomponent> wh = std::make_unique<warhead>(2.0, "WH-" + std::to_string(i), space.remove_object_callback());
    if (small_ship->load_cargo(wh))
      std::clog << "Load warhead " << i + 1 << std::endl;
  }

  //small_ship->add_control_unit(std::make_unique<timed_control>());

//  auto nar = std::make_unique<NAR_M>(0.02, 1.0, "NAR-1",
//                                     plogger{factory_ptr->create_logger("NAR.missile", 1024 * 10), nullptr});
//
//  auto tank = resource_line_factory<fuel_tank_line>::construct_line(1, 100, 100, "", plogger{}, 4, 5000, 1, 1);
//  while (tank->push(100))
//    ;
//
//  auto battery = resource_line_factory<battery_line>::construct_line(1, 50, 50, "", plogger{}, 4, 5000, 1);
//  while (battery->push(100))
//    ;
//
//  fuel_pipe p(tank, battery, 0.01);
//  wire w(battery);
//
//  bool is_charged = false, is_refueled = false;
//  while (!is_charged || !is_refueled) {
//    if (!is_charged) is_charged = nar->charge(w);
//    if (!is_refueled) is_refueled = nar->refule(p);
//  }

  space.add_object(std::move(small_ship), {0, 0}, {0.0, 0.0});

  std::thread t([&space]() {
    std::this_thread::sleep_for(20s);

//    nar->arm();
//    space.add_object(std::move(nar), {0, 0}, {5.0, 0.0});

    std::this_thread::sleep_for(1200s);
    std::this_thread::sleep_for(30s);
    space.stop();
  });

  space.run();
  t.join();
  return 0;
}