//
// Created by yalavrinenko on 30.08.2019.
//

#include "time_controller.hpp"
#include "../control_events/base_events.hpp"
std::vector<pcontrol_event> timed_control::control(timestamp const &ts) {
  std::vector<pcontrol_event> events;
  if (ts == timestamp(100)) {
    auto start_event = std::make_unique<ship_events::reactor_start_event>("Master PU");
    events.emplace_back(std::move(start_event));
  }

  if (ts == timestamp(150)) {
    auto start_event = std::make_unique<ship_events::reactor_start_event>("Slave PU");
    events.emplace_back(std::move(start_event));
  }


  if (ts == timestamp(1000)){
    events.emplace_back(new ship_events::engine_rotation_event("Master Engine", {1.0, 1.0}));
    events.emplace_back(new ship_events::engine_rotation_event("Slave Engine 0", {0.0, 1.0}));
    events.emplace_back(new ship_events::engine_rotation_event("Slave Engine 1", {-1.0, 0.0}));
  }

  if (ts == timestamp(1500)){
    events.emplace_back(new ship_events::engine_fire_event("Master Engine", ship_events::engine_fire_event::action_set::ignite));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 0", ship_events::engine_fire_event::action_set::ignite));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 1", ship_events::engine_fire_event::action_set::ignite));
  }

  if (ts == timestamp(2000)){
    events.emplace_back(new ship_events::engine_fire_event("Master Engine", ship_events::engine_fire_event::action_set::shutdown));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 0", ship_events::engine_fire_event::action_set::shutdown));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 1", ship_events::engine_fire_event::action_set::shutdown));
  }

  if (ts == timestamp(3000)){
    events.emplace_back(new ship_events::engine_fire_event("Master Engine", ship_events::engine_fire_event::action_set::ignite));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 0", ship_events::engine_fire_event::action_set::ignite));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 1", ship_events::engine_fire_event::action_set::ignite));
  }

  if (ts == timestamp(3500)){
    events.emplace_back(new ship_events::engine_fire_event("Master Engine", ship_events::engine_fire_event::action_set::shutdown));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 0", ship_events::engine_fire_event::action_set::shutdown));
    events.emplace_back(new ship_events::engine_fire_event("Slave Engine 1", ship_events::engine_fire_event::action_set::shutdown));
  }
  return events;
}
