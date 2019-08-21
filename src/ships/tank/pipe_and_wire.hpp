//
// Created by yalavrinenko on 21.08.2019.
//

#ifndef DSCS_PIPE_AND_WIRE_HPP
#define DSCS_PIPE_AND_WIRE_HPP
#include "fuel_adapter.hpp"
#include "battery_adapter.hpp"

template<class adapter_type>
class ipipe{
public:
  ipipe(std::shared_ptr<adapter_type> linked_line, pbattery_line electric_supply, double charge_consumption):
    charge_consumption_(charge_consumption), linked_line_(std::move(linked_line)),
    electric_supply_line_(std::move(electric_supply)){
  }

  virtual double pull(double q){
    return linked_line_->pull(evaluate_q(q));
  }

  virtual double push(double q){
    return linked_line_->push(evaluate_q(q));
  }

  pbattery_line electric_line() { return electric_supply_line_;  }

  void set_electric_line(pbattery_line const &new_line){
    electric_supply_line_ = new_line;
  }

protected:
  double required_charge(double q){ return q * charge_consumption_;  }

  double evaluate_q(double q){
    auto charge = (electric_supply_line_) ? electric_supply_line_->pull(required_charge(q)) : 0.0;
    if (charge_consumption_ != 0.0)
      q = charge / charge_consumption_;
    return q;
  }

  std::shared_ptr<adapter_type> linked_line_;

private:
  double charge_consumption_;
  pbattery_line electric_supply_line_;
};

class fuel_pipe : public ipipe<fuel_tank_line>{
public:
  fuel_pipe(pfuel_tank_line linked_line,
            pbattery_line electric_supply, double charge_consumption)
      : ipipe(std::move(linked_line), std::move(electric_supply),
              charge_consumption) {}
};

class wire : public ipipe<battery_line>{
public:
  explicit wire(pbattery_line linked_line): ipipe(std::move(linked_line), nullptr, 0.0){
  }

  double occupancy() const {
    return linked_line_->capacity() / linked_line_->max_capacity();
  }
};
#endif // DSCS_PIPE_AND_WIRE_HPP
