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
  double power_consumption_per_thrust;

  double thrust_align_speed;
  double align_charge;
};

enum class engine_state{
  active,
  disable
};

class iengine: public icomponent{
public:
  iengine(double mass, engine_option const &option, std::string const &name, plogger logger);

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

protected:
  virtual void align_engine();

  engine_option option_;

  double current_thrust_{0};
  engine_state current_state_{0};

  mutable double current_fuel_comsumption_{0};
  mutable double current_power_consumption_{0};

  vector_2d current_thrust_direction_{0, 1};
  vector_2d target_thrust_direction_{0, 1};
private:
};

class small_engine: public iengine{
public:
  small_engine(engine_option const &option, std::string const &name, plogger logger):
    iengine(10000.0, option, name, std::move(logger)){
  }
};
#endif // DSCS_ENGINE_HPP
