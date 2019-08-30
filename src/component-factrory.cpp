//
// Created by yalavrinenko on 26.08.2019.
//

#include "component-factrory.hpp"

double component_scale(ComponentSize size){
  switch (size){
  case ComponentSize ::tiny:
    return 0.25;
  case ComponentSize ::small:
    return 0.5;
  case ComponentSize ::normal:
    return 1.0;
  case ComponentSize ::big:
    return 2.0;
  case ComponentSize ::huge:
    return 4.0;
  default:
    throw UnknownComponentSize("Unknown component size");
  }
}