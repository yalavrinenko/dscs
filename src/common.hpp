//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_COMMON_HPP
#define DSCS_COMMON_HPP
#include <chrono>
#include <thread>
#include <fstream>
using namespace std::chrono_literals;

struct timestamp{
public:
  timestamp(): old_point_(std::chrono::high_resolution_clock::now()){
  }

  double now() const {
    return time_;
  }

  double delta() const {
    return delta_;
  }

  timestamp& operator++ (){
    time_ += delta_;
    delay_system();
    return *this;
  }

protected:

  void delay_system(){
    auto now_point = std::chrono::high_resolution_clock::now();
    if (now_point - old_point_ > real_time_delta)
      std::this_thread::sleep_for(now_point - (old_point_ + real_time_delta));
    old_point_ = now_point;
  }

  double time_{0.0};
  double const delta_ = 0.0010;
  std::chrono::high_resolution_clock::time_point old_point_;
  std::chrono::milliseconds const real_time_delta{200ms}; //20ms
};

struct vector_2d{
  vector_2d() = default;

  vector_2d(double x, double y): x(x), y(y) {}

  vector_2d operator+ (vector_2d const &v){
    return {this->x + v.x, this->y + v.y};
  }

  vector_2d operator- (vector_2d const &v){
    return {this->x - v.x, this->y - v.y};
  }

  vector_2d& operator+= (vector_2d const &v){
    this->x += v.x; this->y += v.y;
    return *this;
  }

  vector_2d operator* (double v){
    return {this->x * v, this->y * v};
  }

  vector_2d operator/ (double v){
    return *this * (1.0 / v);
  }


  double x{0}, y{0};
};

struct space{
  vector_2d x_axis;
  vector_2d y_axis;
};

#endif // DSCS_COMMON_HPP
