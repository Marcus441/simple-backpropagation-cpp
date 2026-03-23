#include "value.h"
#include <print>

int main() {
  Value v1{2.0}, v2{-3.0}, v3{10.0};
  Value result = v1 * v2 + v3;

  std::println("{:.2f}", result);

  std::print("Children: ");

  auto children = result.prev();
  for (size_t i = 0; i < children.size(); ++i) {
    std::print("{:.2f}", children[i]);
    std::print("{}", (i == children.size() - 1 ? "" : " "));
  }
  std::println("");
  return 0;
}
