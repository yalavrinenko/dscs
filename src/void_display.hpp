//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_DISPLAY_HPP
#define DSCS_VOID_DISPLAY_HPP
#include "interface.hpp"
#include "common.hpp"
#include <fstream>
#include <vector>
#include <filesystem>

std::ofstream& operator << (std::ofstream& out, vector_2d const &v);

class void_display: public idisplay{
public:
  explicit void_display(std::filesystem::path const& path): output(path){
  }

  void dump_object(vector_2d const& p, vector_2d const &v, timestamp const &t);

  ~void_display() override {
    output.close();
  }
private:
  std::ofstream output;
};

#endif // DSCS_VOID_DISPLAY_HPP
