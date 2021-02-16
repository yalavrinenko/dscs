//
// Created by yalavrinenko on 16.02.2021.
//

#ifndef DSCS_WARHEAD_HPP
#define DSCS_WARHEAD_HPP
#include <ships/component.hpp>
#include <void_object.hpp>
#include <functional>

using explode_callback = std::function<void(void_object const* carrier_object_, double explosion_radius)>;

class warhead: public icomponent {
public:
  warhead(double mass, std::string name, auto const* carrier): icomponent(mass, std::move(name), nullptr, component_type::warhead),
                                                      carrier_object_{carrier}, explosion_radius_{mass_to_explosion_r(mass)}{
  }
  void action() override;
  void log_action() const override;
  double mass() const override;
  void draw() override;

private:

  static constexpr double mass_to_explosion_r(double mass) {
      return mass * 0.001;
  }

  void_object const* carrier_object_ = nullptr;
  double explosion_radius_ = {0};
};


#endif//DSCS_WARHEAD_HPP
