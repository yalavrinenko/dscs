//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_DRIFTING_HULK_HPP
#define DSCS_DRIFTING_HULK_HPP
#include "void_object.hpp"
class drifting_hulk: public void_object {
public:
  explicit drifting_hulk(double mass): void_object(mass){
  }
  void update(timestamp const &time) override {}
  vector_2d force(timestamp const &time) override { return vector_2d(); }
};

#endif // DSCS_DRIFTING_HULK_HPP
