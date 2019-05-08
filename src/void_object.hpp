//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_OBJECT_HPP
#define DSCS_VOID_OBJECT_HPP
#include "common.hpp"
#include <memory>

class void_object {
public:
  explicit void_object(double mass): mass_(mass){
  }

  virtual void update(timestamp const &time) = 0;

  virtual vector_2d force(timestamp const &time) = 0;

  virtual ~void_object() = default;

  virtual double mass() const { return mass_; }
protected:
  double mass_;
};

using pvoid_object = std::unique_ptr<void_object>;

#endif // DSCS_VOID_OBJECT_HPP
