#include "value.h"

#include <cmath>
#include <functional>
#include <set>

void Value::Backward() {
  std::vector<Value> topo;
  std::set<const void*> visited;

  std::function<void(Value)> build_topo = [&](Value v) {
    if (visited.find(v.Id()) == visited.end()) {
      visited.insert(v.Id());
      for (auto& child : v.Prev()) {
        build_topo(child);
      }
      topo.push_back(v);
    }
  };
  build_topo(*this);
  this->Grad(1.0);
  for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
    it->m_state_->backward_();
  }
  for (auto& node : topo) {
    node.ClearGraph();
  }
}

Value operator+(const Value& lhs, const Value& rhs) {
  Value result(lhs.m_state_->data_ + rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kAdd;
  std::function<void()> backward = [lhs, rhs, result] {
    lhs.Grad(1.0f * result.Grad());
    rhs.Grad(1.0f * result.Grad());
  };
  result.SetBackward(backward);
  return result;
}

Value operator*(const Value& lhs, const Value& rhs) {
  Value result(lhs.m_state_->data_ * rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kMultiply;
  std::function<void()> backward = [lhs, rhs, result] {
    lhs.Grad(rhs.Data() * result.Grad());
    rhs.Grad(lhs.Data() * result.Grad());
  };
  result.SetBackward(backward);
  return result;
}

Value Value::Tanh() {
  double x = this->Data();
  double tanh_x = (std::exp(2 * x) - 1) / (std::exp(2 * x) + 1);
  Value result = Value(tanh_x, {*this});
  result.m_state_->op_ = Operation::kTanh;

  std::function<void()> backward = [this, tanh_x, result] {
    this->Grad((1.0 - std::pow(tanh_x, 2)) * result.Grad());
  };
  result.SetBackward(backward);
  return result;
}
