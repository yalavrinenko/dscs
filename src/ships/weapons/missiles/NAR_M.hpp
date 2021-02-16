//
// Created by yalavrinenko on 16.02.2021.
//

#ifndef DSCS_NAR_M_HPP
#define DSCS_NAR_M_HPP
#include <ships/weapons/missiles/missile.hpp>

class timed_control;

class NAR_M: public missile {
public:
  NAR_M(double ignite_at, double explode_at, std::string name, plogger logger);
  void arm() override;
  ~NAR_M() override;

private:
  std::unique_ptr<timed_control> controller_;
};


#endif//DSCS_NAR_M_HPP
