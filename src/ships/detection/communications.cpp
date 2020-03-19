//
// Created by yalavrinenko on 03.09.2019.
//

#include "communications.hpp"
void radio_unit::radio_receiver::receive_package(field_package package) {
  this->packet_buffer_.emplace_back(std::move(package));
}
void radio_unit::send_message(std::string message, double power) {
  auto message_charge = message.size() * option_.charge_per_byte;
  auto transmission_charge = power * option_.charge_per_distance;

  auto charge = option_.power_supply.pull(message_charge + transmission_charge);
  auto target_power = power * (charge / (message_charge + transmission_charge));

  communication_data comm_data(std::move(message), option_.frequency, target_power);
  transmission_field_.send_to_field(std::move(comm_data));

  power_consumtion_ = target_power;
}
