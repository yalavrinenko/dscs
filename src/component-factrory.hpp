//
// Created by yalavrinenko on 26.08.2019.
//

#ifndef DSCS_COMPONENT_FACTRORY_HPP
#define DSCS_COMPONENT_FACTRORY_HPP

#include "ships/engines/engine.hpp"
#include "ships/power/reactor.hpp"

class UnknownComponentSize: public std::exception{
public:
  explicit UnknownComponentSize(std::string text):  message(std::move(text)){}

  [[nodiscard]]
  const char *what() const noexcept override { return message.c_str(); }
private:
  std::string message;
};

template <class ComponentType, class ComponentOptionType>
class ComponentFactory{
public:
  template <typename scale_type>
  static std::shared_ptr<ComponentType> construct(fuel_pipe const &pipe,
      wire const &out_wire,
      std::string name,
      plogger logger){
    return std::make_shared<ComponentType>(ComponentType::default_mass() * scale_type::scale,
                                           ComponentOptionType::make_default(scale_type::scale, out_wire, pipe),
                                           std::move(name), std::move(logger));
  }

    template <typename scale_type>
    static std::shared_ptr<ComponentType> construct(fuel_pipe const &pipe,
                                                    wire const &out_wire,
                                                    std::string name,
                                                    pbattery_line apu,
                                                    plogger logger){
        return std::make_shared<ComponentType>(ComponentType::default_mass() * scale_type::scale,
                                               ComponentOptionType::make_default(scale_type::scale, out_wire, pipe),
                                               std::move(name), std::move(apu), std::move(logger));
    }
};

using ReactorFactory = ComponentFactory<reactor, reactor_option>;
using EngineFactory = ComponentFactory<engine, engine_option>;


#endif // DSCS_COMPONENT_FACTRORY_HPP
