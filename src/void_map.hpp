//
// Created by yalavrinenko on 03.04.19.
//

#ifndef DSCS_VOID_MAP_HPP
#define DSCS_VOID_MAP_HPP

#include <list>
#include <vector>
#include "void_object.hpp"
#include "void_display.hpp"
#include "ships/control/actions.hpp"
#include "signal_env.hpp"

class void_map: std::enable_shared_from_this<void_map> {
public:
  explicit void_map(std::shared_ptr<logger_factory> log_factory):
    logger_factory_(std::move(log_factory)),
    sig_env_(shared_from_this()){
    display = std::make_unique<void_display>(logger_factory_->create_logger("void_display.map"));
  }

  virtual void add_object(pvoid_object &&object, vector_2d const &position,
                          vector_2d const &velocity);

  virtual void update();

  void set_display(std::unique_ptr<void_display> &&d){
    display = std::move(d);
  }

  void run();

  void stop() {
    exit_ = true;
  }

protected:
  void update_objects();

  void integrate();

  void dump() const;

  void update_actions(timestamp const &time);

  struct void_object_description{
    pvoid_object object;
    vector_2d position;
    vector_2d velocity;
    void_object_description(pvoid_object &&object, vector_2d const &p, vector_2d const &v):
        object(std::move(object)), position(p), velocity(v){
    }
  };

  void_object_description const& find_object(void_object const* ptr) const;

  std::vector<void_object_description> objects;
  action_queue actions_;
  timestamp time;

  signal_environment sig_env_;

  mutable std::unique_ptr<void_display> display;
  std::shared_ptr<logger_factory> logger_factory_;

  bool exit_ = false;

  friend signal_environment;
};

#endif // DSCS_VOID_MAP_HPP
