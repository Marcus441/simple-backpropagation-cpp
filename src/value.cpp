#include "value.h"

Value operator+(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data + rhs.m_state->data, {lhs, rhs});
  return result;
}

Value operator*(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data * rhs.m_state->data, {lhs, rhs});
  return result;
}
