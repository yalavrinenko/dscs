//
// Created by yalavrinenko on 07.04.19.
//

#ifndef DSCS_SMALL_HPP
#define DSCS_SMALL_HPP

#include "hull.hpp"
#include "engines/engine.hpp"
#include "power/reactor.hpp"
#include "tank/pipe_and_wire.hpp"

class small : public ship_hull{
public:
  small(std::string name, plogger logger);

protected:
  std::shared_ptr<reactor> master_core_, slave_core_;

  std::shared_ptr<engine> main_engine_;
  std::shared_ptr<engine> slave_engine_[2];


};

#endif // DSCS_SMALL_HPP
