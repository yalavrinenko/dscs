//
// Created by yalavrinenko on 19.10.2019.
//

#ifndef DSCS_IFIELD_HPP
#define DSCS_IFIELD_HPP

#include <cstdint>
#include <vector>
#include "../common.hpp"
#include <optional>

class communication_data{
public:
  explicit communication_data(size_t frequency): freq_(frequency), data_{}{}

  [[nodiscard]] std::vector<uint8_t> const& data() const { return data_; }
protected:
  std::vector<uint8_t> data_;

private:
  size_t freq_;
};

struct field_package{

  field_package(communication_data data, vector_2d location): data_(std::move(data)),
                                                   location_(location){
  }

  enum class type{
    original, reflected
  };

  [[nodiscard]]
  vector_2d source_location(vector_2d const &reciver_location) const{
    return (location_ - reciver_location).in_polar();
  }

  [[nodiscard]]
  field_package reflect_from(vector_2d const &location) const{
    auto reflected_signal = field_package(this->data_, location);
    reflected_signal.direction = field_package::type ::reflected;
    return reflected_signal;
  }

  [[nodiscard]] type package_type() const { return direction;  }

  [[nodiscard]] communication_data data() const { return data_; }
private:
  vector_2d const location_;
  type direction{type::original};
  communication_data data_;
};

class ifield{
public:
  virtual void send_to_field(field_package package) = 0;


};

#endif // DSCS_IFIELD_HPP
