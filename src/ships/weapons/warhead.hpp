//
// Created by yalavrinenko on 16.02.2021.
//

#ifndef DSCS_WARHEAD_HPP
#define DSCS_WARHEAD_HPP
#include <ships/component.hpp>
#include <void_object.hpp>
#include <functional>

class warhead: public icomponent {
public:
  using explode_callback = std::function<void(void_object const* carrier_object_, double explosion_radius)>;

  warhead(double mass, std::string name, auto const *carrier, warhead::explode_callback explode)
      : icomponent(mass, std::move(name), plogger(), component_type::warhead),
        call_explosion_(std::move(explode)),
        carrier_object_{carrier}, explosion_radius_{mass_to_explosion_r(mass)} {}

  void action() override;
  void log_action() const override;
  double mass() const override;
  void draw() override;

  void explode();

private:

  static constexpr double mass_to_explosion_r(double mass) {
      return mass * 0.001;
  }

  explode_callback call_explosion_ = nullptr;
  void_object const* carrier_object_ = nullptr;
  double explosion_radius_ = {0};
};


#endif//DSCS_WARHEAD_HPP
