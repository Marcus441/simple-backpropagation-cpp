#include <neuron.hpp>

#include <cassert>

#include "value.hpp"

std::mt19937 Neuron::rng(std::random_device{}());
std::uniform_real_distribution<double> Neuron::dist(-1.0, 1.0);

Neuron::Neuron(int n_inputs) {
  b_ = dist(rng);
  for (int i = 0; i < n_inputs; ++i) {
    w_.emplace_back(dist(rng));
  }
};

auto Neuron::operator()(const std::vector<Value>& x) const -> Value {
  assert(x.size() == w_.size());
  Value weight_sum;
  for (size_t i = 0; i < w_.size(); ++i) {
    weight_sum = weight_sum + (x[i] * w_[i]);
  }
  Value activation = (weight_sum + b_).Tanh();
  return activation;
}
