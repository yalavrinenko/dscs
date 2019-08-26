//
// Created by yalavrinenko on 26.08.2019.
//

#include "component-factrory.hpp"
std::shared_ptr<reactor> ReactorFactory::construct(fuel_pipe const &fuel,
                                                   wire const &out_wire,
                                                   std::string name,
                                                   double scale,
                                                   plogger logger) {
  return std::make_shared<reactor>(10.0 * scale * 2.0,
      base_option(fuel, out_wire, scale), std::move(name), std::move(logger));
}
std::shared_ptr<engine> EngineFactory::construct(fuel_pipe const &fuel,
                                                 wire const &out_wire,
                                                 std::string name, double scale,
                                                 plogger logger) {
  return std::make_shared<engine>(10 * scale,
      base_option(fuel, out_wire, scale),
      std::move(name), std::move(logger));
}
