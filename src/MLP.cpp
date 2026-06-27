#include "MLP.hpp"

#include <memory>
#include <vector>

MLP::MLP(std::initializer_list<LayerSize> sizes) {
  parameters_ = std::make_shared<std::vector<Value>>();
  for (const auto& s : sizes) {
    layers_.emplace_back(s);
    auto layer_params = layers_.back().Parameters().lock();
    parameters_->insert(parameters_->end(), layer_params->begin(), layer_params->end());
  }
}

Value MLP::operator()(const std::vector<double>& x) {
  std::vector<Value> input;
  input.reserve(x.size());
  for (double d : x) {
    input.emplace_back(d);
  }
  for (const auto& layer : layers_) {
    input = layer(input);
  }
  return input[0];
}
