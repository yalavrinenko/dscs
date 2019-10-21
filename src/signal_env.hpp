//
// Created by yalavrinenko on 10.09.2019.
//

#ifndef DSCS_SIGNAL_ENV_HPP
#define DSCS_SIGNAL_ENV_HPP

#include "ships/control/actions.hpp"
#include "ships/control/control_events/base_events.hpp"
#include "ships/detection/communications.hpp"
#include "void_object.hpp"
#include <memory>
class void_map;

class signal_receiver {
public:
  void recive_signla(comm_data &&data, vector_2d location){
    received_datas_.emplace(comm_data_pair{std::move(data), location});
  }

protected:
  using comm_data_pair = std::pair<comm_data, vector_2d>;
  std::queue<comm_data_pair> received_datas_;
};

class signal_environment{
public:
  explicit signal_environment(std::shared_ptr<void_map> void_map):
    void_map_(std::move(void_map)){
  }

  void send_signal(comm_data &&data, void_object* sender);

  std::vector<control_action> extract_control_actions(timestamp const &ts);

protected:

  std::shared_ptr<void_map> void_map_;
  std::queue<comm_signal> signals_;
  std::vector<std::shared_ptr<signal_receiver>> recievers_;
};

#endif // DSCS_SIGNAL_ENV_HPP
