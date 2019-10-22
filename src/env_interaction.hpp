//
// Created by yalavrinenko on 19.10.2019.
//

#ifndef DSCS_ENV_INTERACTION_HPP
#define DSCS_ENV_INTERACTION_HPP

#include <memory>
#include "fields/em_field.hpp"

class void_environment{
public:
  [[nodiscard]] em_field & EM_Field() { return em_field_; }

protected:
  em_field em_field_;
};

using pvoid_environment = std::shared_ptr<void_environment>;

#endif // DSCS_ENV_INTERACTION_HPP
