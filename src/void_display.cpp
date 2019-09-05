//
// Created by yalavrinenko on 03.04.19.
//

#include "void_display.hpp"

void void_display::dump_object(vector_2d const &p, vector_2d const &v,
                               timestamp const &t, std::string const &name) {
  logger()->log(t.now(), p.x, p.y, v.x, v.y, name);
}
