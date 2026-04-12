#include <vector>

#include "MLP.hpp"
#include "util/graphing.hpp"

auto main() -> int {
  MLP mlp({{.n_in_ = 3, .n_out_ = 4}, {.n_in_ = 4, .n_out_ = 4}, {.n_in_ = 4, .n_out_ = 1}});

  std::vector<double> x{2, 3, -1};

  util::graphing::ExportToDot(mlp(x), "file.dot");

  return 0;
}
