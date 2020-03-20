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
  auto gui = gui::logger_factory::create("Space ship1");

  void_map space(factory_ptr);

  Random r;
  int N = 10;
//  for (auto i = 0; i < N; ++i){
//    auto hulk = std::make_unique<drifting_hulk>(r.uniform(1000, 10000));
//    space.add_object(std::move(hulk),
//        r.uniform_v(-1000, 1000),
//        r.uniform_v(-0.01, 0.01));
//  }

  auto small_ship = std::make_unique<small>("Ship_S", factory_ptr->create_logger("S1.ship", 1024*10));
  small_ship->add_control_unit(std::make_unique<timed_control>());
  space.add_object(std::move(small_ship), r.uniform_v(-1000, 1000), {0.0, 0.0});

  std::thread t([&space](){
    std::this_thread::sleep_for(1200s);
    space.stop();
  });

  space.run();
  t.join();
  return 0;
}