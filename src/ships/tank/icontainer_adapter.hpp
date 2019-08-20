//
// Created by yalavrinenko on 03.06.19.
//

#ifndef DSCS_ICONTAINER_ADAPTER_HPP
#define DSCS_ICONTAINER_ADAPTER_HPP
#include "../component.hpp"
#include <vector>

template <class accumulative_tank_type>
class itank_adapter : public icomponent {
public:
  explicit itank_adapter(double mass, double input_power, double output_power)
      : icomponent(mass), MAX_INPUT(input_power), MAX_OUTPUT(output_power) {}

  void action() override {}

  double mass() const override { return icomponent::mass(); }

  virtual void push(double q){

  }

  virtual double pull(double q){

  }

protected:
  using stored_container_ptr = std::shared_ptr<accumulative_tank_type>;
  std::vector<stored_container_ptr> inputs;

private:
  double const MAX_INPUT;
  double const MAX_OUTPUT;
};
#endif // DSCS_ICONTAINER_ADAPTER_HPP
