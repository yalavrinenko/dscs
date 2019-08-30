//
// Created by yalavrinenko on 30.08.2019.
//

#include "base_events.hpp"
control_action ship_events::reactor_start_event::action(control_interface &interface, timestamp const &ts) {
  auto reactor_core = std::dynamic_pointer_cast<reactor>(interface.find_interface("Reactor " + reactor_name_));
  if (reactor_core){
    return control_action([reactor_core](auto const &ts){
      reactor_core->ignite();
    }, ts + 1);
  } else
    return control_action([](auto const &ts){}, ts + 1);
}
ship_events::reactor_start_event::~reactor_start_event() = default;
