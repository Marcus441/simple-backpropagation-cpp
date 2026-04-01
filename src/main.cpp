#include "util/graphing.h"
#include "value.h"
#include <print>

int main() {
  // inputs
  Value x1{2.0};
  x1.label("x1");

  Value x2{0.0};
  x2.label("x2");

  // weights
  Value w1{-3.0};
  w1.label("w1");
  Value w2{1.0};
  w2.label("w2");

  // bias
  Value b{6.8813735870195432};
  b.label("b");

  Value x1w1{x1 * w1};
  x1w1.label("x1 * w1");

  Value x2w2{x2 * w2};
  x2w2.label("x2 * w2");

  Value x1w1x2w2{x1w1 + x2w2};
  x1w1x2w2.label("x1*w1 + x2*w2");
  Value n{x1w1x2w2 + b};
  n.label("n");

  Value o{n.tanh()};
  o.backward();

  util::graphing::export_to_dot(o, "file.dot");
  std::println("");

  return 0;
}
