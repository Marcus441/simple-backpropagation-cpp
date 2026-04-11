#include <random>
#include <vector>

#include "value.hpp"

class Neuron {
 public:
  Neuron(int n_inputs);
  auto operator()(std::vector<double>) -> Value;

 private:
  static std::mt19937 rng;
  static std::uniform_real_distribution<double> dist;
  std::vector<Value> w_;
  Value b_;
};
