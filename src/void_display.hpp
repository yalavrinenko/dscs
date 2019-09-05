//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_DISPLAY_HPP
#define DSCS_VOID_DISPLAY_HPP
#include "interface.hpp"
#include "common.hpp"
#include <fstream>
#include <vector>
#include "logger_factory.hpp"

std::ofstream& operator << (std::ofstream& out, vector_2d const &v);

class void_display: public idisplay{
public:
  explicit void_display(plogger logger): logger_(std::move(logger)){
  }

  void dump_object(vector_2d const &p, vector_2d const &v, timestamp const &t,
                   std::string const &name);

  plogger & logger() { return logger_; }

  ~void_display() override = default;
private:
  plogger logger_;
};

#endif // DSCS_VOID_DISPLAY_HPP
