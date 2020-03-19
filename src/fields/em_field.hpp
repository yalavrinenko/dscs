//
// Created by yalavrinenko on 19.10.2019.
//

#ifndef DSCS_EM_FIELD_HPP
#define DSCS_EM_FIELD_HPP

#include <cstdint>
#include <vector>
#include <queue>
#include "../common.hpp"

#include <map>
#include <optional>

class communication_data{
public:
  explicit communication_data(std::string data,
      double frequency, double power): freq_(frequency), data_{std::move(data)}, power_{power} {}

  [[nodiscard]] std::string const& data() const { return data_; }

  [[nodiscard]] double transmission_freq() const { return freq_; }

  [[nodiscard]] double power() const { return power_; }

protected:
  std::string data_;

private:
  double freq_;
  double power_;
};

struct field_package{

  field_package(communication_data data, double power)
      : data_(std::move(data)), signal_power_(power), transmission_frequency_{data_.transmission_freq()} {}

  enum class type{
    original, reflected
  };

  [[nodiscard]] bool is_origin() const { return package_type() == type ::original; }

  [[nodiscard]]  vector_2d& source_location() { return location_; }

  [[nodiscard]]  vector_2d const& source_location() const { return location_; }

  [[nodiscard]] field_package reflect() const{
    auto reflected_signal = field_package(this->data_, signal_power_);
    reflected_signal.direction = field_package::type ::reflected;
    return reflected_signal;
  }

  [[nodiscard]] type package_type() const { return direction;  }

  [[nodiscard]] communication_data data() const { return data_; }

  [[nodiscard]] double transmission_frequency() const { return transmission_frequency_; }

  [[nodiscard]] double transmission_power() const { return signal_power_; }
private:
  vector_2d location_{0, 0};
  type direction{type::original};
  communication_data data_;
  double signal_power_;
  double transmission_frequency_;
};

struct signal_receiver{
  virtual void receive_package(field_package package) = 0;
};

class em_field{
public:
  void send_to_field(communication_data data);

  std::queue<communication_data>& package_queue() { return to_send_packages_; }

  void register_receiver(std::shared_ptr<signal_receiver> receiver, double frequency){
    receivers_[frequency] = std::move(receiver);
  }

  void unregister_receiver(double frequency) {
    receivers_.erase(frequency);
  }

protected:
  void recieve_packet(field_package const &package);

private:
  std::queue<communication_data> to_send_packages_;

  std::map<double, std::shared_ptr<signal_receiver>> receivers_;

  friend class void_map;
};


#endif // DSCS_EM_FIELD_HPP
