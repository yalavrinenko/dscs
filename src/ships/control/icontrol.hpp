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
#include <ships/weapons/warhead.hpp>
#include <variant>
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

template<typename ... visitor_t>
class visitor_wrapper: public visitor_t...{
public:
  using visitor_t::operator()...;
};

template<typename ... visitor_t>
visitor_wrapper(visitor_t...) -> visitor_wrapper<visitor_t...>;

class icontrol{
public:
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

  icontrol() = default;

  explicit icontrol(control_interface &ship) {
    construct_component_groups(ship);
  }

  virtual std::vector<control_action> control(timestamp const &ts, control_interface &ship) final{
    if (is_first_entry_) {
      construct_component_groups(ship);
      is_first_entry_ = false;
    }

    return control_impl(ts, ship);
  }

protected:
  virtual std::vector<control_action> control_impl(timestamp const &ts, control_interface &ship) = 0;

  auto& controllers() { return devs_; }

  template <typename group_type, typename component_t, component_type search_type>
  void look_for_component(auto &ship, group_type &egroup){
    for (auto &[name, comp] : ship.components()){
      if (comp->type() == search_type)
        egroup.devices.emplace_back(ship.template find_interface<component_t>(name));
    }
    egroup.ready2use = true;
  };

  void construct_component_groups(control_interface &ship) {
    auto visitor_set = visitor_wrapper{
        [this, &ship](component_group<pengine> &cg) {
          return look_for_component<decltype(cg), engine, component_type::engine>(ship, cg);
        },
        [this, &ship](component_group<preactor> &cg) {
          return look_for_component<decltype(cg), reactor, component_type::reactor>(ship, cg);
        },
        [this, &ship](component_group<pwarhead> &cg) {
          return look_for_component<decltype(cg), warhead, component_type::warhead>(ship, cg);
        }
    };

    for (auto &v : devs_){
      std::visit(visitor_set, v);
    }
  }

  using control_devices = std::variant <
      component_group<pengine>,
      component_group<preactor>,
      component_group<std::shared_ptr<warhead>>
  >;
  std::vector<control_devices> devs_{
      component_group<pengine>{},
      component_group<preactor>{},
      component_group<std::shared_ptr<warhead>>{}
  };

  bool is_first_entry_ = true;
};

#endif // DSCS_ICONTROL_HPP
