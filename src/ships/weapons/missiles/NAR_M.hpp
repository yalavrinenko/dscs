//
// Created by yalavrinenko on 16.02.2021.
//

#ifndef DSCS_NAR_M_HPP
#define DSCS_NAR_M_HPP
#include <ships/weapons/missiles/missile.hpp>

class timed_control;

class NAR_M: public missile {
public:

  struct flight_parameters{
    double ignition_time{};
    double explosion_time{};
    vector_2d alignment_vector;
  };

  NAR_M(std::string name, plogger logger);

  void set_flight_parameter(flight_parameters const& opts);

  void arm() override;
  ~NAR_M() override;

  struct missile_parameters{
    static constexpr double ignite_time = 0.1;
    static constexpr double peak_velocity = 2.0;
  };

private:
  std::unique_ptr<timed_control> controller_;
};


#endif//DSCS_NAR_M_HPP
