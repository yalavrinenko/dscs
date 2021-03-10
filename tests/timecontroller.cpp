//
// Created by yalavrinenko on 10.03.2021.
//
#include <src/ships/control/control_units/time_controller.hpp>
#include <ranges>
int main(int argc, char** argv){
  timed_control tc;

  auto ignite = [](icontrol::component_group<pengine> &eng){
    std::clog << "Ignite engine" << std::endl;
  };

  auto explode = [](icontrol::component_group<pwarhead> &wh){
    std::clog << "Explode warhead" << std::endl;
  };

  tc.add_action(timestamp(10), ignite);
  tc.add_action(timestamp(20), explode);

  control_interface ship;

  for (std::weakly_incrementable auto i : std::views::iota(0, 30)){
    auto acts = tc.control(timestamp(i), ship);
    for (auto &a : acts)
      a.run(timestamp(i));
  }

  return 0;
}