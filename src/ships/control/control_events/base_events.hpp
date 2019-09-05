//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_BASE_EVENTS_HPP
#define DSCS_BASE_EVENTS_HPP

#include "../icontrol.hpp"

namespace ship_events{
class reactor_start_event: public control_event{
public:
  explicit reactor_start_event(std::string reactor_name)
      : reactor_name_(std::move(reactor_name)), reactor_id_{} {}

  explicit reactor_start_event(size_t reactor_id)
      : reactor_name_{}, reactor_id_(reactor_id) {}

  control_action action(control_interface &interface, timestamp const &ts) override;

  ~reactor_start_event() override;
private:
  std::string const reactor_name_;
  std::optional<size_t> const reactor_id_;
};

class engine_rotation_event: public control_event{
public:
  engine_rotation_event(std::string name, vector_2d direction):
    engine_{std::move(name)}, direction_{std::move(direction)}{}

  control_action action(control_interface &interface,
                        timestamp const &ts) override;

private:
  std::string const engine_;
  vector_2d const direction_;
};

class engine_fire_event: public control_event{
public:
  enum class action_set{
    ignite,
    shutdown
  };

  engine_fire_event(std::string name, action_set act):
      engine_{std::move(name)}, act_{act}{}

  control_action action(control_interface &interface,
                        timestamp const &ts) override;

private:
  std::string const engine_;
  action_set const act_;
};
}

#endif // DSCS_BASE_EVENTS_HPP
