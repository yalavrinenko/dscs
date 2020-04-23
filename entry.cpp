//
// Created by yalavrinenko on 03.04.19.
//
#include "src/drifting_hulk.hpp"
#include "src/ships/control/control_units/time_controller.hpp"
#include "src/ships/small.hpp"
#include "src/void_map.hpp"
#include <future>
#include <random>

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

using namespace std::chrono_literals;

int main(int argc,char** argv){
  auto factory_ptr = std::make_shared<logger_factory>("../ramfs");
  auto gui_factory = gui::logger_environment::create();

  void_map space(factory_ptr, gui_factory);

  Random r;
  int N = 10;
  for (auto i = 0; i < N; ++i){
    auto hulk = std::make_unique<drifting_hulk>(r.uniform(1000, 10000));
    space.add_object(std::move(hulk),
        r.uniform_v(-300, 300),
        r.uniform_v(-10, 10));
  }

  auto gui_ship = gui_factory->create_logger("Ship_S");
  auto text_log = factory_ptr->create_logger("S1.ship", 1024*10);
  auto small_ship = std::make_unique<small>("Ship_S", plogger{text_log, gui_ship});
  //small_ship->add_control_unit(std::make_unique<timed_control>());
  space.add_object(std::move(small_ship), r.uniform_v(-300, 300), {0.0, 0.0});

  std::thread t([&space](){
    std::this_thread::sleep_for(1200s);
    space.stop();
  });

  space.run();
  t.join();
  return 0;
}