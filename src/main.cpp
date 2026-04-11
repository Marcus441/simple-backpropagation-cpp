#include <print>
#include <vector>

#include "neuron.hpp"
#include "util/graphing.hpp"
#include "value.hpp"

auto main() -> int {
  std::vector<double> input_data{2.0, 3.0};
  Neuron n(2);
  Value o = n(input_data);
  std::print("{:.2f}", o.Data());

  util::graphing::ExportToDot(o, "file.dot");

  return 0;
}
