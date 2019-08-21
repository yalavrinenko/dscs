//
// Created by yalavrinenko on 03.06.19.
//

#ifndef DSCS_ICONTAINER_ADAPTER_HPP
#define DSCS_ICONTAINER_ADAPTER_HPP
#include "../component.hpp"
#include <vector>
#include <numeric>
#include <algorithm>

template <class accumulative_tank_type>
class itank_adapter : public icomponent {
public:
  itank_adapter(double mass, double input_power, double output_power, std::string name, plogger logger)
      : icomponent(mass, std::move(name), std::move(logger)), MAX_INPUT(input_power), MAX_OUTPUT(output_power) {}

  void action() override {}

  [[nodiscard]]
  double mass() const override {
    return icomponent::mass() + std::accumulate(inputs.begin(), inputs.end(), 0.0, [](auto sum, auto &v){
      return sum + v->mass();
    });
  }

  virtual double push(double q){
    if (q > MAX_INPUT)
      q = MAX_INPUT;
    auto input_power = q;

    for (auto i = 0; i < inputs.size() && q > 0; ++i){
      auto pushed = inputs[i]->push(q);
      q -= pushed;
      if (q < 0) q = 0;
    }

    return input_power - q;
  }

  virtual double pull(double q){
    if (q > MAX_OUTPUT)
      q = MAX_OUTPUT;

    auto output_amount = 0.0;
    for (auto i = 0; i < inputs.size() && output_amount < q; ++i){
      auto required = q - output_amount;
      if (required < 0) required = 0;
      output_amount += inputs[i]->pull(required);
    }

    return output_amount;
  }

  void log_action() const override{
    this->logger()->log("Adapter [", this->name() + "]:");
    for (auto const& container: inputs)
      container->log_action();
    this->logger()->log("Mass:", this->mass());
  }

  template <class ... TArgs>
  void add_tank(TArgs ... args){
    inputs.emplace_back(std::make_shared<accumulative_tank_type>(args..., logger()));
  }

protected:
  using stored_container_ptr = std::shared_ptr<accumulative_tank_type>;
  std::vector<stored_container_ptr> inputs;

private:
  double const MAX_INPUT;
  double const MAX_OUTPUT;
};

#endif // DSCS_ICONTAINER_ADAPTER_HPP
