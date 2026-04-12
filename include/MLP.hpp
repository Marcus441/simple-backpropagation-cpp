#include <initializer_list>
#include <vector>

#include "layer.hpp"

class MLP {
 public:
  MLP(std::initializer_list<LayerSize> sizes);
  auto operator()(std::vector<double> x) -> Value;

 private:
  std::vector<Layer> layers_;
};
