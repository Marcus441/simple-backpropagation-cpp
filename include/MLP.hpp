#include <initializer_list>
#include <memory>
#include <vector>

#include "layer.hpp"

class MLP {
 public:
  MLP(std::initializer_list<LayerSize> sizes);
  Value operator()(const std::vector<double>& x);
  [[nodiscard]] std::weak_ptr<std::vector<Value>> Parameters() const { return parameters_; }

 private:
  std::vector<Layer> layers_;
  std::shared_ptr<std::vector<Value>> parameters_;
};
