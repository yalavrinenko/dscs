//
// Created by yalavrinenko on 22.10.2019.
//
#include "em_field.hpp"
#include <iostream>

void em_field::send_to_field(communication_data data) {
  to_send_packages_.push(std::move(data));
}

void em_field::recieve_packet(field_package const &package) {
  if (receivers_.count(package.transmission_frequency()))
    receivers_[package.transmission_frequency()]->receive_package(package);
}
