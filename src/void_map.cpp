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

  update_objects();

  integrate();

  dump();
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
    display->dump_object(o.position, o.velocity, time);
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
