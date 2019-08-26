//
// Created by yalavrinenko on 07.04.19.
//

#include "small.hpp"
small::small(std::string name, plogger logger):
  ship_hull(1e+6, std::move(name), std::move(logger)){

}
