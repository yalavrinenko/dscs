//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_HULL_HPP
#define DSCS_HULL_HPP
#include "../void_object.hpp"
#include <numeric>
#include <algorithm>
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

  vector_2d force(timestamp const &time) override { return vector_2d(); }

  ~ship_hull() override = default;

protected:
  void add_component(pscomponent component) {
    hull_components_.emplace_back(std::move(component));
  }

  std::vector<pscomponent> hull_components_;
};

#endif // DSCS_HULL_HPP
