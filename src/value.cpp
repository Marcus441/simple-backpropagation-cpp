#include "value.h"

std::ostream &operator<<(std::ostream &os, const Value &v) {
  return os << v.data;
}

Value operator+(const Value &lhs, const Value &rhs) {
  return lhs.data + rhs.data;
}

Value operator*(const Value &lhs, const Value &rhs) {
  return lhs.data * rhs.data;
}
