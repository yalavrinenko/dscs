//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_OBJECT_HPP
#define DSCS_VOID_OBJECT_HPP
#include "common.hpp"
#include <memory>
#include "logger_factory.hpp"
#include "ships/component.hpp"
#include "ships/control/actions.hpp"

class void_object:  public icomponent{
public:
  explicit void_object(double mass, std::string name, plogger logger):
    icomponent(mass, std::move(name), std::move(logger)){
  }

  virtual void update(timestamp const &time) = 0;

  virtual vector_2d force(timestamp const &time) = 0;

  virtual std::vector<control_action> extract_control_actions(timestamp const &ts) = 0;

  virtual ~void_object() = default;
};

using pvoid_object = std::unique_ptr<void_object>;

#endif // DSCS_VOID_OBJECT_HPP
