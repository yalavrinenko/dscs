//
// Created by yalavrinenko on 03.04.19.
//

#include "void_display.hpp"

std::ofstream& operator << (std::ofstream& out, vector_2d const &v){
  out << v.x << "\t" << v.y;
  return out;
}

void void_display::dump_object(vector_2d const &p, vector_2d const &v,
                               timestamp const &t) {
  output << t.now() << "\t";
  output << p << "\t";
  output << v << std::endl;
}
