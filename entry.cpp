//
// Created by yalavrinenko on 03.04.19.
//
#include "src/void_display.hpp"
#include "src/void_map.hpp"
#include "src/drifting_hulk.hpp"
#include <filesystem>
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
  std::filesystem::create_directories("./logs");
  auto void_d = std::make_unique<void_display>("./logs/void.map");

  void_map space;
  space.set_display(std::move(void_d));

  Random r;
  int N = 10;
  for (auto i = 0; i < N; ++i){
    auto hulk = std::make_unique<drifting_hulk>(r.uniform(1000, 10000));
    space.add_object(std::move(hulk), r.uniform_v(-1000, 1000), r.uniform_v(-0.1, 0.1));
  }
  std::thread t([&space](){
    std::this_thread::sleep_for(20s);
    space.stop();
  });
  space.run();
  t.join();
  return 0;
}