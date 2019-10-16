//
// Created by yalavrinenko on 03.09.2019.
//

#ifndef DSCS_COMMUNICATIONS_HPP
#define DSCS_COMMUNICATIONS_HPP

#include "../../common.hpp"
#include <cstdint>
#include <vector>

class comm_data{
public:
  explicit comm_data(size_t frequency): freq_(frequency), data_{}{}

  [[nodiscard]] std::vector<uint8_t> const& data() const { return data_; }
protected:
  std::vector<uint8_t> data_;

private:
  size_t freq_;
};

#endif // DSCS_COMMUNICATIONS_HPP
