//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_ICONTROL_HPP
#define DSCS_ICONTROL_HPP

#include "actions.hpp"
#include <unordered_map>
#include "../component.hpp"

#include "../engines/engine.hpp"
#include "../power/reactor.hpp"
#include "../tank/fuel_adapter.hpp"
#include "../tank/battery_adapter.hpp"
#include "../detection/communications.hpp"

class control_interface{
public:
  template<class ComponentType>
  std::shared_ptr<ComponentType> find_interface(std::string const &name) const{
    if (interface_map_.count(name))
      return std::dynamic_pointer_cast<ComponentType>(interface_map_.at(name));
    else
      return nullptr;
  }

  void register_interface(std::string name, pscomponent object){
    interface_map_.emplace(std::move(name), std::move(object));
  }

  std::unordered_map<std::string, pscomponent> const& components() const { return interface_map_; }
protected:
  std::unordered_map<std::string, pscomponent> interface_map_;
};

class icontrol{
public:
  icontrol() = default;

  explicit icontrol(control_interface &ship) {
    construct_component_groups(ship);
  }

  virtual std::vector<control_action> control(timestamp const &ts, control_interface &ship) = 0;

protected:
  void construct_component_groups(control_interface &ship) {
    for (auto &[name, comp] : ship.components()){
      if (comp->type() == component_type::engine)
        engines.devices.emplace_back(ship.find_interface<engine>(name));
      if (comp->type() == component_type::reactor)
        reactors.devices.emplace_back(ship.find_interface<reactor>(name));
    }

    engines.ready2use = true;
    reactors.ready2use = true;
  }

  template <typename component_type>
  struct component_group{
    std::vector<component_type> devices;

    template <typename function_type>
    void apply(function_type funcion){
      std::for_each(devices.begin(), devices.end(), funcion);
    }

    template <typename transform_function, typename transform_value>
    std::vector<transform_value> request(transform_function func){
      std::vector<transform_value> v(devices.size());
      std::transform(devices.begin(), devices.end(), v.begin(), func);
      return v;
    }

    bool ready2use = false;
  };

  component_group<pengine> engines;
  component_group<preactor> reactors;
};

#endif // DSCS_ICONTROL_HPP
