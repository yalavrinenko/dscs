//
// Created by yalavrinenko on 17.02.2021.
//

#ifndef DSCS_LAUNCHER_HPP
#define DSCS_LAUNCHER_HPP

#include <component-factrory.hpp>
#include <ships/component.hpp>
#include <ships/weapons/missiles/missile.hpp>

class base_cargo;

class launcher: public icomponent{
public:
  launcher(double mass, std::string name, plogger logger, component_type type, wire power, fuel_pipe pipe,
           size_t launch_pads);

  void action() override;
  void log_action() const override;
  void draw() override;

  void connect_cargo(std::shared_ptr<base_cargo> cargo);

  ~launcher() override;
protected:
  std::shared_ptr<base_cargo> cargo_;
  wire power_;
  fuel_pipe fuel_;

  size_t active_pads_{0};
  size_t launch_pads_;

  double power_consumption_{};
  double fuel_consumption_{};

  enum missile_action{
    idle,
    refuel,
    charge,
    fire
  };

  struct missile_info{
    std::unique_ptr<missile> missile_ptr;
    bool is_armed{false};
    bool is_locked{false};
    bool is_ready2fire{false};

    std::atomic<missile_action> next_action;
  };

  std::vector<missile_info> loaded_;

};


#endif//DSCS_LAUNCHER_HPP
