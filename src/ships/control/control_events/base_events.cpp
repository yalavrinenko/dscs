//
// Created by yalavrinenko on 30.08.2019.
//

#include "base_events.hpp"
#include "../../power/reactor.hpp"
#include "../../engines/engine.hpp"

control_action ship_events::reactor_start_event::action(control_interface &interface,
    timestamp const &ts) {
  auto reactor_core = interface.find_interface<reactor>("Reactor " + reactor_name_);
  if (reactor_core){
    return control_action([reactor_core](auto const &ts){
      reactor_core->ignite();
    }, ts + 1);
  } else
    return default_action(ts);
}
ship_events::reactor_start_event::~reactor_start_event() = default;

control_action
ship_events::engine_rotation_event::action(control_interface &interface,
                                           timestamp const &ts) {
  auto engine_unit = interface.find_interface<engine>("Engine " + engine_);
  if (engine_unit){
    return {
      [engine_unit, this](auto const &ts){
        engine_unit->align(this->direction_);
      },
      ts + 1
    };
  }
  return default_action(ts);
}

control_action
ship_events::engine_fire_event::action(control_interface &interface,
                                       timestamp const &ts) {
  auto engine_unit = interface.find_interface<engine>("Engine " + engine_);
  if (engine_unit){
    return {
        [engine_unit, this](auto const &ts){
          if (act_ == action_set::ignite)
            engine_unit->ignite();
          else
            engine_unit->shutdown();
        },
        ts + 1
    };
  }
  return default_action(ts);
}
