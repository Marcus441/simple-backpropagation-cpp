#include <memory>
#include <vector>

#include "neuron.hpp"
#include "value.hpp"

struct LayerSize {
  int n_in_;
  int n_out_;
};
class Layer {
 public:
  explicit Layer(LayerSize s);
  std::vector<Value> operator()(const std::vector<Value>& x) const;
  [[nodiscard]] auto Parameters() const -> std::weak_ptr<std::vector<Value>> {
    return parameters_;
  };

 private:
  std::vector<Neuron> neurons_;
  std::shared_ptr<std::vector<Value>> parameters_;
};
