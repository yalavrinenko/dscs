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

  propagate_signals(time);

  update_actions(time);

  dump();

  //logger_factory_->flush_loggers();
  if (gui_factory_){
    gui_factory_->draw();
  }

  while (actions_.invoke_next(time));

  remove_queued_objects();
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
    o.object->draw();
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

void void_map::update_actions(timestamp const &ts) {
  for (auto &o : objects) {
    actions_.push_actions(o.object->extract_control_actions(ts));
  }
}

const void_map::void_object_description & void_map::find_object(void_object const *ptr) const {
  return *std::find_if(objects.begin(), objects.end(), [ptr](void_object_description const &obj){
    return obj.object.get() == ptr;
  });
}

void void_map::propagate_signals(timestamp const &ts) {
  for (auto &object : objects){
    auto &message_queue = object.object->env_link().EM_Field().package_queue();
    while (!message_queue.empty()){
      auto comm_message = message_queue.front(); message_queue.pop();
      auto comm_package = field_package(comm_message, comm_message.power());
      this->actions_.push_actions(signal_propagate_action(comm_package, object.position, ts));
    }
  }
}

std::vector<control_action> void_map::signal_propagate_action(field_package comm_package,
                                                 vector_2d sender_location, timestamp const &ts) {
  std::vector<control_action> signal_transmission_action;
  for (auto ts_shift = 0; ts_shift < std::ceil(comm_package.transmission_power()); ++ts_shift) {
    auto next_ts = ts + ts_shift;
    std::pair range{ts_shift * PhysUnit::EM_SPEED(), (ts_shift + 1) * PhysUnit::EM_SPEED()};

    auto propagate_action = [sender_location, range,  comm_package,
                             this](timestamp const &ts) mutable {
      for (auto &object : objects) {
        auto distance = (object.position - sender_location).len();
        if (range.first < distance && distance <= range.second) {
          comm_package.source_location() = (object.position - sender_location).in_polar();
          object.object->env_link().EM_Field().recieve_packet(comm_package);

          if (comm_package.package_type() == field_package::type::original) {
            auto reflected_package = comm_package.reflect();
            auto reflection_actions = this->signal_propagate_action(reflected_package, object.position, ts);
            this->actions_.push_actions(reflection_actions);
          }
        }
      }
    };

    signal_transmission_action.emplace_back(propagate_action, next_ts);
  }

  return signal_transmission_action;
}

std::function<void(void_object const*, double)> void_map::remove_object_callback() {
  return [this](void_object const* main_object, double r){
    auto position = find_object(main_object).position;
    std::for_each(objects.begin(), objects.end(), [this, position, r](void_object_description const& obj){
      if ((position - obj.position).len() <= r)
        remove_object_queue_.insert(obj.object.get());
    });
  };
}

void void_map::remove_queued_objects() {
  if (remove_object_queue_.empty()) return;

  auto remove_it = std::remove_if(objects.begin(), objects.end(), [this](void_object_description const&obj){
    return remove_object_queue_.count(obj.object.get());
  });

  objects.erase(remove_it, objects.end());

  remove_object_queue_.clear();
}
std::function<void(pvoid_object, const vector_2d &)> void_map::add_object_callback(const void_object *launcher) {
  return [this, parent = launcher](auto obj, auto const& v){
    auto position = find_object(parent).position;
    add_object(std::move(obj), position, v);
  };
}
