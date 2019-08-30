//
// Created by yalavrinenko on 30.08.2019.
//

#ifndef DSCS_BASE_EVENTS_HPP
#define DSCS_BASE_EVENTS_HPP

#include "../icontrol.hpp"
#include "../../power/reactor.hpp"

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
}

#endif // DSCS_BASE_EVENTS_HPP
