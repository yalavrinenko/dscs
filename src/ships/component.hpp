//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_COMPONENT_HPP
#define DSCS_COMPONENT_HPP
#include <memory>

class icomponent{
public:
  explicit icomponent(double mass): mass_(mass){
  }

  virtual void action() = 0;

  virtual double mass() const {
    return mass_;
  }
protected:
  double mass_;
};

using pcomponent = std::unique_ptr<icomponent>;
#endif // DSCS_COMPONENT_HPP
