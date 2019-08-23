//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_REACTOR_HPP
#define DSCS_REACTOR_HPP
#include "../component.hpp"
#include "../tank/battery_adapter.hpp"
#include "../tank/pipe_and_wire.hpp"
#include <list>

using namespace std::string_literals;

struct reactor_option {
  struct {
    double mass;
    double capacity;
    double in_power;
    double out_power;
  } apu;

  fuel_pipe nuclear_fuel;
  wire output_wire;

  double peak_output;

  double fuel_per_charge;

  double ignition_speed;
  double ignition_charge_consumption;
};

enum class reactor_state{
  shutdown,
  ignition,
  active,
};

namespace std{
std::string to_string(reactor_state const &state);
}

class reactor : public icomponent {
public:
  reactor(double mass, reactor_option const &option, std::string const &name,
          plogger logger);

  void action() override {
    for (auto &component: components_)
      component->action();

    if (state_ == reactor_state::ignition){
      ignite_action();
    }

    if (state_ == reactor_state::active){
      active_action();
    }

    if (state_ == reactor_state::shutdown){
      current_power_ = charge_consumption_ = fuel_consumtion_ = 0.0;
    }
  }

  void log_action() const override;

  double mass() const override { return icomponent::mass() + apu_->mass(); }

  virtual void ignite();

  virtual void shutdown(){
    state_ = reactor_state ::shutdown;
  }

  reactor_state state() const { return state_; }

  virtual ~reactor();
protected:
  virtual void ignite_action();

  virtual void active_action();

  reactor_option option_;

  pbattery_line apu_;
  pbattery_line tmp_power_line_ = nullptr;

  double current_power_{0};
  double next_power_{0};
  double fuel_consumtion_{0};
  double charge_consumption_{0};
  reactor_state state_{reactor_state::shutdown};

  std::vector<pscomponent> components_;
};

#endif // DSCS_REACTOR_HPP
