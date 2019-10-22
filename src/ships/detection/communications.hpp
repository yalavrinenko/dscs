//
// Created by yalavrinenko on 03.09.2019.
//

#ifndef DSCS_COMMUNICATIONS_HPP
#define DSCS_COMMUNICATIONS_HPP

#include "../../common.hpp"
#include "../../fields/em_field.hpp"
#include "../component.hpp"
#include "../tank/pipe_and_wire.hpp"
#include <cstdint>
#include <vector>

struct radio_transmitter_option{
  wire power_supply;

  double charge_per_byte = 0.01;
  double charge_per_distance = 1.0;

  double frequency = 44110;
};

class radio_transmitter : public icomponent{
public:
  radio_transmitter(double mass, radio_transmitter_option option, em_field &field):
      option_{std::move(option)},
      transmission_field_{field},
      icomponent(mass, "radio", nullptr){
    signal_receiver_ = std::make_shared<radio_receiver>(received_packages);
    transmission_field_.register_receiver(signal_receiver_, option.frequency);
  }

  void send_message(std::string message, double power);

  std::vector<field_package> const & received_messages() const { return  received_packages; }

  void action() override {
    power_consumtion_ = 0;
  }

  void log_action() const override {

  }

  double linked_frequency() const { return option_.frequency; }

  double power_consumption() const { return power_consumtion_; }

  void flush() {
    received_packages.clear();
  }
protected:
  struct radio_receiver: public signal_receiver{
    explicit radio_receiver(std::vector<field_package> &buffer): packet_buffer_{buffer}{}

    void receive_package(field_package package) override;

  protected:
    std::vector<field_package> &packet_buffer_;
  };

  std::shared_ptr<signal_receiver> signal_receiver_ = nullptr;

  std::vector<field_package> received_packages;

  radio_transmitter_option option_;

  em_field& transmission_field_;

  double power_consumtion_ {0};
};

#endif // DSCS_COMMUNICATIONS_HPP
