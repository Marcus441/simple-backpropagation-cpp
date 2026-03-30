#include "value.h"
#include <cmath>
#include <functional>
#include <set>

void Value::backward() {
  std::vector<Value> topo;
  std::set<const void *> visited;

  std::function<void(Value)> build_topo = [&](Value v) {
    if (visited.find(v.id()) == visited.end()) {
      visited.insert(v.id());
      for (auto &child : v.prev()) {
        build_topo(child);
      }
      topo.push_back(v);
    }
  };
  build_topo(*this);
  this->grad(1.0);
  for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
    it->m_state->backward();
  }
}

Value operator+(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data + rhs.m_state->data, {lhs, rhs});
  result.m_state->op = Operation::ADD;
  std::function<void()> backward = [lhs, rhs, result] {
    lhs.grad(1.0f * result.grad());
    rhs.grad(1.0f * result.grad());
  };
  result.set_backward(backward);
  return result;
}

Value operator*(const Value &lhs, const Value &rhs) {
  Value result(lhs.m_state->data * rhs.m_state->data, {lhs, rhs});
  result.m_state->op = Operation::MULTIPLY;
  std::function<void()> backward = [lhs, rhs, result] {
    lhs.grad(rhs.data() * result.grad());
    rhs.grad(lhs.data() * result.grad());
  };
  result.set_backward(backward);
  return result;
}

Value Value::tanh() {
  double x = this->data();
  double tanh_x = (std::exp(2 * x) - 1) / (std::exp(2 * x) + 1);
  Value result = Value(tanh_x, {*this});
  result.m_state->op = Operation::TANH;

  std::function<void()> backward = [this, tanh_x, result] {
    this->grad((1.0 - std::pow(tanh_x, 2)) * result.grad());
  };
  result.set_backward(backward);
  return result;
}
