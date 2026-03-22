#include "value.h"

std::ostream &operator<<(std::ostream &os, const Value &v) {
  return os << v.m_state->data;
}

Value operator+(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data + rhs.m_state->data, {lhs, rhs});
  return result;
}

Value operator*(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data * rhs.m_state->data, {lhs, rhs});
  return result;
}
