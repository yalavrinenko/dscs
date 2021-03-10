//
// Created by yalavrinenko on 17.02.2021.
//

#ifndef DSCS_LAUNCHER_HPP
#define DSCS_LAUNCHER_HPP

#include <component-factrory.hpp>
#include <ships/component.hpp>

class base_cargo;

class launcher: public icomponent{
public:
  launcher(double mass, std::string name, plogger logger, component_type type, wire power, fuel_pipe pipe,
           size_t launch_pads);

  void action() override;
  void log_action() const override;
  void draw() override;

  ~launcher() override;
protected:
  std::unique_ptr<base_cargo> cargo_;
  wire power_;
  fuel_pipe fuel_;
  size_t launch_pads_;

  enum state{
    idle,
    refuel,
    charge,
    redy_to_fire
  };

  double power_consumption_{};
  double fuel_consumption_{};
};


#endif//DSCS_LAUNCHER_HPP
