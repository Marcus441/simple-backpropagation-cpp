#include "layer.hpp"

#include <memory>
#include <vector>

#include "value.hpp"

Layer::Layer(LayerSize s) {
  parameters_ = std::make_shared<std::vector<Value>>();
  for (int i = 0; i < s.n_out_; ++i) {
    neurons_.emplace_back(s.n_in_);
    auto neuron_params = neurons_.back().Parameters().lock();
    parameters_->insert(parameters_->end(), neuron_params->begin(), neuron_params->end());
  }
}

std::vector<Value> Layer::operator()(const std::vector<Value>& x) const {
  std::vector<Value> out;
  out.reserve(neurons_.size());
  for (const auto& neuron : neurons_) {
    out.emplace_back(neuron(x));
  }
  return out;
}
