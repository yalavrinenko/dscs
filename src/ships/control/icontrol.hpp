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
  pscomponent find_interface(std::string const &name) const{
    if (interface_map_.count(name))
      return interface_map_.at(name);
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
};

using pcontrol_event = std::unique_ptr<control_event>;

class icontrol{
public:
  virtual std::vector<pcontrol_event> control(timestamp const &ts) = 0;
};


#endif // DSCS_ICONTROL_HPP
