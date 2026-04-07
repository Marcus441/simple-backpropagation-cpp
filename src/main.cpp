#include <print>

#include "util/graphing.h"
#include "value.h"

int main() {
  // inputs
  Value x1{2.0};
  x1.Label("x1");

  Value x2{0.0};
  x2.Label("x2");

  // weights
  Value w1{-3.0};
  w1.Label("w1");
  Value w2{1.0};
  w2.Label("w2");

  // bias
  Value b{6.8813735870195432};
  b.Label("b");

  Value x1w1{x1 * w1};
  x1w1.Label("x1 * w1");

  Value x2w2{x2 * w2};
  x2w2.Label("x2 * w2");

  Value x1w1x2w2{x1w1 + x2w2};
  x1w1x2w2.Label("x1*w1 + x2*w2");
  Value n{x1w1x2w2 + b};
  n.Label("n");

  Value o{n.Tanh()};
  o.Backward();

  util::graphing::ExportToDot(o, "file.dot");
  std::println("");

  return 0;
}
