#include "MLP.hpp"

#include <vector>

MLP::MLP(std::initializer_list<LayerSize> sizes) {
  for (const auto& s : sizes) {
    layers_.emplace_back(s);
  }
}

auto MLP::operator()(std::vector<double> x) -> Value {
  std::vector<Value> input;
  for (double d : x) {
    input.emplace_back(d);
  }
  for (const auto& layer : layers_) {
    input = layer(input);
  }
  return input[0];
}
