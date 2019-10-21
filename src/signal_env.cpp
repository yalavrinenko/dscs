//
// Created by yalavrinenko on 10.09.2019.
//

#include "signal_env.hpp"
#include "void_map.hpp"

vector_2d signal_environment::comm_signal::source_location(
    vector_2d const &reciver_location) const {

}
signal_environment::comm_signal
signal_environment::comm_signal::reflect_from(vector_2d const &location) const {

}

void signal_environment::send_signal(comm_data &&data, void_object *sender) {
  vector_2d location = this->void_map_->find_object(sender).position;

  comm_signal signal(std::move(data), location);
  signal.direction = comm_signal::type::original;

  signals_.push(std::move(signal));
}

std::vector<control_action>
signal_environment::extract_control_actions(timestamp const &ts) {
  while (!signals_.empty()){

  }
}