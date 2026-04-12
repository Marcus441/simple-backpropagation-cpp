#include "layer.hpp"

#include <vector>

#include "value.hpp"

Layer::Layer(LayerSize s) {
  for (int i = 0; i < s.n_out_; ++i) {
    neurons_.emplace_back(s.n_in_);
  }
}

auto Layer::operator()(const std::vector<Value>& x) const -> std::vector<Value> {
  std::vector<Value> out;
  for (const auto& neuron : neurons_) {
    out.emplace_back(neuron(x));
  }
  return out;
}
