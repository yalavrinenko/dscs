//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_COMMON_HPP
#define DSCS_COMMON_HPP
#include <chrono>
#include <thread>
#include <fstream>
#include <cmath>
using namespace std::chrono_literals;

struct PhysUnit {
   inline static double EM_SPEED() { return 100.0; }
};

struct timestamp{
public:
  timestamp(): old_point_(std::chrono::high_resolution_clock::now()){
  }

  explicit timestamp(size_t step): time_(step * delta_()), step_(step),
    old_point_(std::chrono::high_resolution_clock::now()){
  }

  [[nodiscard]] double now() const {
    return time_;
  }

  [[nodiscard]] static double delta() {
    return delta_();
  }

  [[nodiscard]] size_t step() const {
    return step_;
  }

  timestamp& operator++ (){
    time_ += delta_();
    ++step_;
    delay_system();
    return *this;
  }

  timestamp operator+ (int v) const{
    timestamp new_ts(*this);
    new_ts.time_ += delta_() * v;
    new_ts.step_ += v;
    return new_ts;
  }

  timestamp operator- (timestamp const& rhs) const {
    return timestamp(step_ - rhs.step_);
  }

  bool operator < (timestamp const &ts) const { return this->step() < ts.step();  }

  bool operator == (timestamp const &ts) const { return this->step() == ts.step(); }
protected:

  void delay_system(){
    auto now_point = std::chrono::high_resolution_clock::now();
    if (now_point - old_point_ < real_time_delta())
      std::this_thread::sleep_for(real_time_delta() - (now_point - old_point_));
    old_point_ = now_point;
  }

  double time_{0.0};
  size_t step_ {0};
  std::chrono::high_resolution_clock::time_point old_point_;

  static std::chrono::milliseconds real_time_delta() {return 50ms; };
  static double delta_() { return 0.0010; };
};

struct vector_2d{
  vector_2d() = default;

  vector_2d(double x, double y): x(x), y(y) {}

  vector_2d operator+ (vector_2d const &v) const{
    return {this->x + v.x, this->y + v.y};
  }

  vector_2d operator- (vector_2d const &v) const{
    return {this->x - v.x, this->y - v.y};
  }

  vector_2d& operator+= (vector_2d const &v){
    this->x += v.x; this->y += v.y;
    return *this;
  }

  vector_2d operator* (double v) const{
    return {this->x * v, this->y * v};
  }

  vector_2d operator*(vector_2d const &v) const{
    return {this->x * v.x, this->y * v.y};
  }

  vector_2d operator/ (double v) const{
    return *this * (1.0 / v);
  }

  void norm() {
    auto l = len();
    x /= l;
    y /= l;
  }

  [[nodiscard]] double len() const{
    return std::sqrt(x*x + y*y);
  }

  [[nodiscard]] vector_2d in_polar() const {
    auto norm_this = *this; norm_this.norm();
    return {this->len(), norm_this.len() * std::atan2(norm_this.y, norm_this.x)};
  }

  double x{0}, y{0};
};

struct polar_vector: public vector_2d{
  using vector_2d::vector_2d;
  using vector_2d::operator*;

  polar_vector(vector_2d const& rhs): vector_2d(rhs) {}
  polar_vector& operator=(polar_vector const &rhs){
    x = rhs.x;
    y = rhs.y;
    return *this;
  }
  polar_vector& operator=(vector_2d const& rhs) {
    x = rhs.x; y = rhs.y;
    return *this;
  }

  [[nodiscard]] auto distance(polar_vector const& rhs) const {
    return std::sqrt(r() * r() + rhs.r() * rhs.r() - 2.0 * r() * rhs.r() * std::cos(rhs.phi() - rhs.phi()));
  }

  [[nodiscard]] auto in_cartesian() const {
    return vector_2d{r() * std::cos(phi()), r() * std::sin(phi())};
  }

  operator std::pair<double, double> (){
    return {r(), phi()};
  }

  double& r() { return x; }
  double const& r() const { return x; }

  double& phi() { return y; }
  double const& phi() const { return y; }

};

struct space{
  vector_2d x_axis;
  vector_2d y_axis;
};

#endif // DSCS_COMMON_HPP
