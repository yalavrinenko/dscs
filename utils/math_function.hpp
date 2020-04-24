//
// Created by yalavrinenko on 24.04.2020.
//

#ifndef DSCS_MATH_FUNCTION_HPP
#define DSCS_MATH_FUNCTION_HPP
#include <cmath>

class MathExtension{
public:
  static inline double pi() { static auto v = 4.0 * std::atan(1.0);  return v;}
  static inline double dec2rad(double dec){ return dec * MathExtension::pi() / 180.0; }
};

#endif//DSCS_MATH_FUNCTION_HPP
