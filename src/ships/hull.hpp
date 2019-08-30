//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_HULL_HPP
#define DSCS_HULL_HPP
#include "../void_object.hpp"
#include <numeric>
#include <algorithm>
#include "control/icontrol.hpp"

class ship_hull: public void_object{
public:
  ship_hull(double hull_mass, std::string name, plogger logger):
      void_object(hull_mass, std::move(name), std::move(logger)){}

  void action() override {
    for (auto &c : hull_components_)
      c->action();
  }

  void log_action() const override {
    logger()->log("Ship ", name());
    logger()->log("Mass:", mass());
    for (auto &c : hull_components_)
      c->log_action();
  }

  double mass() const override { return icomponent::mass() +
    std::accumulate(hull_components_.begin(), hull_components_.end(), 0, [](auto sum, auto &v){
      return v->mass();
    }); }

  void update(timestamp const &time) override {
    action();
  }

  void add_control_unit(std::unique_ptr<icontrol> unit) {
    control_unit_ = std::move(unit);
  }

  std::vector<control_action> extract_control_actions(timestamp const &ts){
    auto events = control_unit_->control(ts);
    std::vector<control_action> actions;
    std::transform(events.begin(), events.end(), std::back_inserter(actions), [this, &ts](auto const& e){
      return e->action(this->command_interface_, ts);
    });
    return actions;
  }

  ~ship_hull() override = default;

protected:
  void add_component(pscomponent component, bool is_interface=false) {
    hull_components_.emplace_back(std::move(component));
    if (is_interface){
      command_interface_.register_interface(hull_components_.back()->name(),
                                            hull_components_.back());
    }
  }

  control_interface command_interface_;

  std::unique_ptr<icontrol> control_unit_;

  std::vector<pscomponent> hull_components_;
};

#endif // DSCS_HULL_HPP
