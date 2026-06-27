#include <memory>
#include <random>
#include <vector>

#include "value.hpp"

class Neuron {
 public:
  explicit Neuron(int n_inputs);
  Value operator()(const std::vector<Value>& x) const;

  [[nodiscard]] std::weak_ptr<std::vector<Value>> Parameters() const { return parameters_; }

 private:
  static std::mt19937 rng;
  static std::uniform_real_distribution<double> dist;
  std::shared_ptr<std::vector<Value>> parameters_;
  std::vector<Value> w_;
  Value b_;
};
