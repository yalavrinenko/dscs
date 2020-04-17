//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_REACTOR_HPP
#define DSCS_REACTOR_HPP
#include "../../../utils/gui_entries.hpp"
#include "../component.hpp"
#include "../tank/battery_adapter.hpp"
#include "../tank/pipe_and_wire.hpp"
#include <list>

using namespace std::string_literals;

struct reactor_option {
  fuel_pipe nuclear_fuel;
  wire output_wire;

  double peak_output;

  double fuel_per_charge;

  double ignition_speed;
  double ignition_charge_consumption;

  static reactor_option make_default(double scale, wire const &out_wire,
                                     fuel_pipe const &fuel) {
    return {fuel,
            out_wire,
            20.0 * scale,
            0.01 * scale,
            0.05 * scale,
            10.0 * scale};
  }
};

enum class reactor_state{
  shutdown,
  ignition,
  active,
};

namespace std{
std::string to_string(reactor_state const &state);
}

class reactor : public icomponent{
public:
  reactor(double mass, reactor_option const &option, std::string const &name, pbattery_line apu,
          plogger logger);

  void draw() override;
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

  double set_max_power(double new_power_level) {
    auto old_level = option_.peak_output;
    option_.peak_output = new_power_level;
    if (option_.peak_output > 2.0 * initial_max_output_power_)
      option_.peak_output = initial_max_output_power_ * 2.0;
    return old_level;
  }

  virtual void ignite();

  virtual void shutdown(){
    state_ = reactor_state ::shutdown;
  }

  reactor_state state() const { return state_; }

  virtual ~reactor();

  static constexpr double default_mass(){ return 20.0;  }
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

  double const initial_max_output_power_;

  struct{
    int text;
    int num;
  } guis_;
};

using preactor = std::shared_ptr<reactor>;

#endif // DSCS_REACTOR_HPP
