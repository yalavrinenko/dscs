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
#include "env_interaction.hpp"
#include "utils/guilogger.hpp"
#include <unordered_set>

class void_map: std::enable_shared_from_this<void_map> {
public:
  explicit void_map(std::shared_ptr<logger_factory> log_factory, std::shared_ptr<gui::logger_environment> gui_factory):
    logger_factory_(std::move(log_factory)),
    gui_factory_{std::move(gui_factory)}{
    display = std::make_unique<void_display>(logger_factory_->create_logger("void_display.map"));
  }

  virtual void add_object(pvoid_object &&object, vector_2d const &position,
                          vector_2d const &velocity);

  virtual std::function<void(void_object const*, double)> remove_object_callback();

  virtual void update();

  void set_display(std::unique_ptr<void_display> &&d){
    display = std::move(d);
  }

  void run();

  void stop() {
    exit_ = true;
  }

protected:
  void remove_queued_objects();

  void update_objects();

  void integrate();

  void dump() const;

  void update_actions(timestamp const &time);

  void propagate_signals(timestamp const &ts);

  std::vector<control_action> signal_propagate_action(field_package package, vector_2d location, timestamp const &ts);

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

  std::unordered_set<void_object const*> remove_object_queue_;

  mutable std::unique_ptr<void_display> display;
  std::shared_ptr<logger_factory> logger_factory_;
  std::shared_ptr<gui::logger_environment> gui_factory_;

  bool exit_ = false;

  friend class void_environment;
  friend class em_field;
};

#endif // DSCS_VOID_MAP_HPP
