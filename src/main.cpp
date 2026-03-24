#include "util/graphing.h"
#include "value.h"
#include <print>

int main() {
  // inputs
  MAKE_VALUE(x1, 2.0)
  MAKE_VALUE(x2, 0.0)

  // weights
  MAKE_VALUE(w1, -3.0)
  MAKE_VALUE(w2, 1.0)

  // bias
  MAKE_VALUE(b, 6.8813735870195432)

  MAKE_VALUE(x1w1, x1 * w1)
  MAKE_VALUE(x2w2, x2 * w2)

  MAKE_VALUE(x1w1x2w2, x1w1 + x2w2)
  MAKE_VALUE(n, x1w1x2w2 + b)

  MAKE_VALUE(o, n.tanh())

  std::print("{}: {}", o.label(), o.data());

  util::graphing::export_to_dot(o, "file.dot");
  std::println("");

  return 0;
}
