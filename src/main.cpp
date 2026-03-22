#include "value.h"

#include <iostream>
#include <sstream>

int main() {
  Value v1{2.0};
  Value v2{-3.0};
  Value v3{10};

  Value result = v1 * v2 + v3;
  std::cout << "Result = " << result.data() << '\n';
  std::ostringstream out;
  out << "Children: ";
  for (auto child : result.prev()) {
    out << child.data() << " ";
  }
  std::cout << out.view() << '\n';
  return 0;
}
