#include "neuron.hpp"

#include <cassert>
#include <memory>
#include <vector>

#include "value.hpp"

std::mt19937 Neuron::rng(std::random_device{}());
std::uniform_real_distribution<double> Neuron::dist(-1.0, 1.0);

Neuron::Neuron(int n_inputs) {
  parameters_ = std::make_shared<std::vector<Value>>();
  parameters_->reserve(n_inputs + 1);

  parameters_->emplace_back(dist(rng));
  b_ = parameters_->at(0);

  for (int i = 0; i < n_inputs; ++i) {
    parameters_->emplace_back(dist(rng));
    w_.push_back(parameters_->back());
  }
}

Value Neuron::operator()(const std::vector<Value>& x) const {
  assert(x.size() == parameters_->size() - 1);

  // Initialize with the bias
  Value weight_sum = (*parameters_)[0];

  for (size_t i = 0; i < x.size(); ++i) {
    // Offset by 1 to skip the bias in the parameters vector
    weight_sum = weight_sum + (x[i] * (*parameters_)[i + 1]);
  }

  return weight_sum.Tanh();
}
