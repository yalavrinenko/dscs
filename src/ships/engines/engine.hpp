//
// Created by yalavrinenko on 21.08.2019.
//

#ifndef DSCS_ENGINE_HPP
#define DSCS_ENGINE_HPP

#include "../component.hpp"
#include "../../common.hpp"
#include "../tank/pipe_and_wire.hpp"

struct engine_option{
  double max_thrust;
  double ignition_speed;

  struct{
    wire energy_supply;
    fuel_pipe fuel_supply;
  } inputs;

  double fuel_consumption_per_thrust;
  double power_consumption_per_fuel;

  double thrust_align_speed;
  double align_charge;

  static engine_option make_default(double scale, wire const &out_wire,
                                    fuel_pipe const &fuel) {
    return {250.0 * scale, 5.0 * scale,  {out_wire, fuel}, 0.25 * scale,
            0.4 * scale,   0.005 / scale, 0.5 * scale};
  }
};

enum class engine_state{
  active,
  disable
};

class engine : public icomponent{
public:
  engine(double mass, engine_option const &option, std::string const &name, plogger logger);

  void action() override;

  void log_action() const override;

  double mass() const override;

  void align(vector_2d const &direction) {
    target_thrust_direction_ = direction;
    target_thrust_direction_.norm();
  }

  virtual void ignite() {
    current_state_ = engine_state ::active;
    current_thrust_ = 0;
  }

  virtual void shutdown() {
    current_state_ = engine_state ::disable;
    current_thrust_ = 0;
  }

  double align_angle() const {
    auto cos = current_thrust_direction_.x * target_thrust_direction_.x +
               current_thrust_direction_.y * target_thrust_direction_.y;
    return std::acos(cos) * 180.0 / M_PI;
  }

  vector_2d const& thrust_direction() const { return current_thrust_direction_; }

  double thrust_level() const { return current_thrust_; }

  vector_2d thrust() const {
    auto norm = current_thrust_direction_;
    norm.norm();
    return norm * current_thrust_;
  }

  static constexpr double default_mass(){ return 10.0;  }
protected:
  virtual void align_engine();

  engine_option option_;

  double current_thrust_{0};
  engine_state current_state_ = engine_state::disable;

  mutable double current_fuel_comsumption_{0};
  mutable double current_power_consumption_{0};

  vector_2d current_thrust_direction_{0, 1};
  vector_2d target_thrust_direction_{0, 1};
private:
};

using pengine = std::shared_ptr<engine>;
#endif // DSCS_ENGINE_HPP
