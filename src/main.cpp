#include "value.h"

#include <iostream>

int main() {
  Value v1{12};
  Value v2{2};
  Value v3{5};
  std::cout << (v1 + v2) * v3;
  return 0;
}
