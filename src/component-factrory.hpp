//
// Created by yalavrinenko on 26.08.2019.
//

#ifndef DSCS_COMPONENT_FACTRORY_HPP
#define DSCS_COMPONENT_FACTRORY_HPP

#include "ships/engines/engine.hpp"
#include "ships/power/reactor.hpp"

class ReactorFactory{
public:

private:
  static reactor_option base_option(fuel_pipe const &fuel,
      wire const &out_wire,
      double scale = 1) {
    return {
        {
            1.0 * scale,
            500.0 * 2 * scale,
            1.0 * scale,
            5.0 * scale
        },
        fuel,
        out_wire,
        10.0 * scale,
        0.01 * scale,
        0.2 * scale,
        0.1 * scale
    };
  }

  static std::shared_ptr<reactor> construct(fuel_pipe const &fuel, wire const &out_wire,
                                            std::string name,
                                            double scale=1,
                                            plogger logger = nullptr);
};


class EngineFactory{
public:

private:
  static engine_option base_option(fuel_pipe const &fuel,
                                    wire const &out_wire,
                                    double scale = 1) {
    return {
      100.0 * scale,
      5.0 * scale,
      {
        out_wire,
        fuel
      },
      1.0 * scale,
      2.0 * scale,
      0.01 / scale,
      0.5 * scale
    };
  }

  static std::shared_ptr<engine> construct(fuel_pipe const &fuel, wire const &out_wire,
                                            std::string name,
                                            double scale=1,
                                            plogger logger = nullptr);
};

#endif // DSCS_COMPONENT_FACTRORY_HPP
