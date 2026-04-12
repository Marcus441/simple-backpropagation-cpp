#include <vector>

#include "neuron.hpp"
#include "value.hpp"

struct LayerSize {
  int n_in_;
  int n_out_;
};
class Layer {
 public:
  Layer(LayerSize s);
  auto operator()(const std::vector<Value>& x) const -> std::vector<Value>;

 private:
  std::vector<Neuron> neurons_;
};
