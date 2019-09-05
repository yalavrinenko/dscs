//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_ICONTROL_HPP
#define DSCS_ICONTROL_HPP

#include "actions.hpp"
#include <unordered_map>
#include "../component.hpp"

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

protected:
  std::unordered_map<std::string, pscomponent> interface_map_;
};

class control_event{
public:
  virtual control_action action(control_interface &interface, timestamp const &ts) = 0;

  virtual ~control_event() = default;

protected:
  auto default_action(timestamp const &ts) { return control_action([](auto const &ts){}, ts + 1); };
};

using pcontrol_event = std::unique_ptr<control_event>;

class icontrol{
public:
  virtual std::vector<pcontrol_event> control(timestamp const &ts) = 0;
};


#endif // DSCS_ICONTROL_HPP
