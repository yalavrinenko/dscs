//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_DISPLAY_HPP
#define DSCS_VOID_DISPLAY_HPP
#include "../utils/logger_factory.hpp"
#include "common.hpp"
#include "interface.hpp"
#include <fstream>
#include <vector>

std::ofstream& operator << (std::ofstream& out, vector_2d const &v);

class void_display: public idisplay{
public:
  explicit void_display(ptext_logger logger): logger_(std::move(logger)){
  }

  void dump_object(vector_2d const &p, vector_2d const &v, timestamp const &t,
                   std::string const &name);

    ptext_logger & logger() { return logger_; }

  ~void_display() override = default;
private:
    ptext_logger logger_;
};

#endif // DSCS_VOID_DISPLAY_HPP
