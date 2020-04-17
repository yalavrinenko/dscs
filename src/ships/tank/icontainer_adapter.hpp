//
// Created by yalavrinenko on 03.06.19.
//

#ifndef DSCS_ICONTAINER_ADAPTER_HPP
#define DSCS_ICONTAINER_ADAPTER_HPP
#include "../../../utils/gui_entries.hpp"
#include "../component.hpp"
#include <algorithm>
#include <numeric>
#include <vector>

template <class accumulative_tank_type>
class itank_adapter : public icomponent {
public:
  itank_adapter(double mass, double input_power, double output_power, std::string name, plogger logger)
      : icomponent(mass, std::move(name), std::move(logger)), MAX_INPUT(input_power), MAX_OUTPUT(output_power) {
      guis_.num = this->add_gui_entry<gui::numeric_entry>(this->name());
      guis_.plot = this->add_gui_entry<gui::moving_plot_entry>(this->name(), 100);
      guis_.hplot = add_gui_entry<gui::histo_plot_entry>(this->name());
  }


  void action() override {
    for (auto &in : inputs)
      in->action();
  }

  double capacity() const {
    return std::accumulate(inputs.begin(), inputs.end(), 0.0, [](auto sum, auto &v){
      return sum + v->capacity();
    });
  }

  double max_capacity() const {
    return std::accumulate(inputs.begin(), inputs.end(), 0.0, [](auto sum, auto &v){
      return sum + v->max_capacity();
    });
  }

  double percent_load() const { return capacity() / max_capacity(); }

  double input_power() const { return MAX_INPUT; }
  double output_power() const { return MAX_OUTPUT; }

  [[nodiscard]]
  double mass() const override {
    return icomponent::mass() + std::accumulate(inputs.begin(), inputs.end(), 0.0, [](auto sum, auto &v){
      return sum + v->mass();
    });
  }

  virtual double push(double q){
    if (q > MAX_INPUT)
      q = MAX_INPUT;
    auto input_power = q;

    for (auto i = 0; i < inputs.size() && q > 0; ++i){
      auto pushed = inputs[i]->push(q);
      q -= pushed;
      if (q < 0) q = 0;
    }

    return input_power - q;
  }

  virtual double pull(double q){
    if (q > MAX_OUTPUT)
      q = MAX_OUTPUT;

    auto output_amount = 0.0;
    for (auto i = 0; i < inputs.size() && output_amount < q; ++i){
      auto required = q - output_amount;
      if (required < 0) required = 0;
      output_amount += inputs[i]->pull(required);
    }

    return output_amount;
  }

  void log_action() const override{
    this->logger()->log("Adapter [", this->name() + "]:");
    struct {
      double current{0};
      double max{0};
      double consumption{0};
    } info{};

    std::for_each(inputs.begin(), inputs.end(), [&info](auto const &v){
      info.current += v->capacity();
      info.max += v->max_capacity();
      info.consumption += v->consumption();
    });

    this->logger()->log("\t(left/total/consumption)", "(", info.current, "/", info.max, "/", info.consumption, ")");
    this->logger()->log("\tMass:", this->mass());
  }

  void draw() override {
    auto num = this->entry<gui::numeric_entry>(guis_.num);
    auto plot = this->entry<gui::moving_plot_entry>(guis_.plot);
    num->log("Mass:", this->mass());

    struct {
      double current{0};
      double max{0};
      double consumption{0};
      std::vector<double> amount{};
    } info{};
    info.amount.resize(inputs.size());

    std::for_each(inputs.begin(), inputs.end(), [&info](auto const &v){
      info.current += v->capacity();
      info.max += v->max_capacity();
      info.consumption += v->consumption();
    });
    auto selection_function = [this](int i) -> double{
      if (i == -1)
        return inputs.size();
      else
        return inputs[i]->capacity() / inputs[i]->max_capacity();
    };

    num->log("Total:", info.current, info.max);
    plot->log("Flow:", info.consumption);
    entry<gui::histo_plot_entry>(guis_.hplot)->log("Banks", selection_function);
  }

  template <class ... TArgs>
  void add_tank(TArgs ... args){
    inputs.emplace_back(std::make_shared<accumulative_tank_type>(args..., slogger()));
  }

protected:
  using stored_container_ptr = std::shared_ptr<accumulative_tank_type>;
  std::vector<stored_container_ptr> inputs;

  struct {
    int num;
    int plot;
    int hplot;
  } guis_;
private:
  double const MAX_INPUT;
  double const MAX_OUTPUT;
};

template<class tank_type>
using ptank_adapter = std::shared_ptr<itank_adapter<tank_type>>;

template <typename container_type>
class resource_line_factory{
public:
  template<typename ... bank_args>
  static std::shared_ptr<container_type> construct_line(double line_mass, double in_q,
                                      double out_q, std::string name,
                                      plogger logger, size_t bank_count,
                                      bank_args ... args){
    auto container = std::make_shared<container_type>(line_mass, in_q, out_q, std::move(name), std::move(logger));
    for (auto i = 0ull; i < bank_count; ++i){
      container->add_tank(args...);
    }
    return container;
  }
};
#endif // DSCS_ICONTAINER_ADAPTER_HPP
