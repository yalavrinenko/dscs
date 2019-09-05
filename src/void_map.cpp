//
// Created by yalavrinenko on 03.04.19.
//

#include "void_map.hpp"
void void_map::add_object(pvoid_object &&object, vector_2d const &position,
                          vector_2d const &velocity) {
  objects.emplace_back(std::move(object), position, velocity);
}
void void_map::update() {
  ++time;

  while (actions_.invoke_next(time));

  update_objects();

  integrate();

  dump();

  update_actions(time);

  logger_factory_->flush_loggers();
}

void void_map::integrate() {
  for (auto &o : objects){
    auto acceleration = o.object->force(time) / o.object->mass();
    o.velocity += acceleration * time.delta();
    o.position += o.velocity * time.delta();
  }
}

void void_map::dump() const{
  for (auto const &o: objects){
    o.object->log_action();
    display->dump_object(o.position, o.velocity, time, o.object->name());
  }
}

void void_map::run() {
  while (!exit_){
    update();
  }
}

void void_map::update_objects() {
  for (auto &o : objects){
    o.object->update(time);
  }
}
void void_map::update_actions(timestamp const &time) {
  for (auto &o : objects) {
    actions_.push_actions(o.object->extract_control_actions(time));
  }
}
