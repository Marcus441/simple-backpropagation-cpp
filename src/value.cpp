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
  this->m_state_->grad_ = 1.0;
  for (auto it = topo.rbegin(); it != topo.rend(); ++it) {
    it->m_state_->backward_(it->Grad());
  }
}

Value operator+(const Value& lhs, const Value& rhs) {
  Value result(lhs.m_state_->data_ + rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kAdd;

  std::function<void(double)> backward = [lhs, rhs](double out_grad) {
    lhs.GradRef() += 1.0f * out_grad;
    rhs.GradRef() += 1.0f * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

Value operator*(const Value& lhs, const Value& rhs) {
  Value result(lhs.m_state_->data_ * rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kMultiply;

  std::function<void(double)> backward = [lhs, rhs](double out_grad) {
    lhs.GradRef() += rhs.Data() * out_grad;
    rhs.GradRef() += lhs.Data() * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

Value Value::Tanh() {
  double x = this->Data();
  double tanh_x = std::tanh(x);
  Value result = Value(tanh_x, {*this});
  result.m_state_->op_ = Operation::kTanh;

  std::function<void(double)> backward = [*this, tanh_x](double out_grad) {
    this->GradRef() += (1.0 - std::pow(tanh_x, 2)) * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

Value operator*(double lhs, const Value& rhs) {
  Value out(lhs * rhs.Data());
  std::function<void(double)> backward = [lhs, rhs](double out_grad) {
    rhs.GradRef() += lhs * out_grad;
  };
  out.SetBackward(backward);
  return out;
}

Value operator*(const Value& lhs, double rhs) {
  return rhs * lhs;
}

Value operator+(double lhs, const Value& rhs) {
  Value out(lhs + rhs.Data());
  std::function<void(double)> backward = [rhs](double out_grad) { rhs.GradRef() += out_grad; };
  out.SetBackward(backward);
  return out;
}

Value operator+(const Value& lhs, double rhs) {
  return rhs + lhs;
}
