#include "value.h"
#include <cmath>

Value operator+(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data + rhs.m_state->data, {lhs, rhs});
  result.m_state->op = Operation::ADD;
  return result;
}

Value operator*(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data * rhs.m_state->data, {lhs, rhs});
  result.m_state->op = Operation::MULTIPLY;
  return result;
}

Value Value::tanh() {
  double x = this->data();
  double tanh_x = (std::exp(2 * x) - 1) / (std::exp(2 * x) + 1);
  Value result = Value(tanh_x, {*this});
  result.m_state->op = Operation::TANH;
  return result;
}
