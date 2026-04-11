#include "value.hpp"

#include <cmath>
#include <functional>
#include <ranges>
#include <set>

void Value::Backward() {
  std::vector<Value> topo;
  std::set<const void*> visited;

  std::function<void(Value)> build_topo = [&](Value v) -> void {
    if (!visited.contains(v.Id())) {
      visited.insert(v.Id());
      for (auto& child : v.Prev()) {
        build_topo(child);
      }
      topo.push_back(v);
    }
  };
  build_topo(*this);
  this->m_state_->grad_ = 1.0;
  for (auto& it : std::ranges::reverse_view(topo)) {
    it.m_state_->backward_(it.Grad());
  }
}

auto operator/(const Value& lhs, const Value& rhs) -> Value {
  double l_data = lhs.Data();
  double r_data = rhs.Data();

  Value out(l_data / r_data, {lhs, rhs});
  out.m_state_->op_ = Operation::kDivide;

  std::function<void(double)> backward = [lhs, rhs](double out_grad) -> void {
    double l = lhs.Data();
    double r = rhs.Data();
    // Quotient Rule: d/dl (l/r) = 1/r  |  d/dr (l/r) = -l / r^2
    lhs.GradRef() += (1.0 / r) * out_grad;
    rhs.GradRef() += (-l / (r * r)) * out_grad;
  };
  out.SetBackward(backward);

  return out;
}

auto operator-(const Value& lhs, const Value& rhs) -> Value {
  Value out(lhs.Data() - rhs.Data(), {lhs, rhs});
  out.m_state_->op_ = Operation::kSubtract;
  std::function<void(double)> backward = [lhs, rhs](double out_grad) -> void {
    lhs.GradRef() += 1.0F * out_grad;
    rhs.GradRef() += -1.0F * out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto operator+(const Value& lhs, const Value& rhs) -> Value {
  Value result(lhs.m_state_->data_ + rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kAdd;

  std::function<void(double)> backward = [lhs, rhs](double out_grad) -> void {
    lhs.GradRef() += 1.0F * out_grad;
    rhs.GradRef() += 1.0F * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

auto operator*(const Value& lhs, const Value& rhs) -> Value {
  Value result(lhs.m_state_->data_ * rhs.m_state_->data_, {lhs, rhs});
  result.m_state_->op_ = Operation::kMultiply;

  std::function<void(double)> backward = [lhs, rhs](double out_grad) -> void {
    lhs.GradRef() += rhs.Data() * out_grad;
    rhs.GradRef() += lhs.Data() * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

auto Value::Tanh() -> Value {
  double x = this->Data();
  double tanh_x = std::tanh(x);
  Value result = Value(tanh_x, {*this});
  result.m_state_->op_ = Operation::kTanh;

  std::function<void(double)> backward = [*this, tanh_x](double out_grad) -> void {
    this->GradRef() += (1.0 - std::pow(tanh_x, 2)) * out_grad;
  };
  result.SetBackward(backward);
  return result;
}

auto Value::Pow(double other) const -> Value {
  Value out(std::pow(this->Data(), other), {*this});
  out.m_state_->op_ = Operation::kPower;
  std::function<void(double)> backward = [*this, other](double out_grad) -> void {
    this->GradRef() += (other * std::pow(this->Data(), other - 1)) * out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto Value::Exp() -> Value {
  double x = this->Data();
  double e = std::exp(x);
  Value result(e, {*this});
  result.m_state_->op_ = Operation::kExp;
  std::function<void(double)> backward = [*this, e](double out_grad) -> void {
    this->GradRef() += e * out_grad;
  };

  result.SetBackward(backward);
  return result;
}

auto operator*(double lhs, const Value& rhs) -> Value {
  Value out(lhs * rhs.Data(), {rhs});
  out.m_state_->op_ = Operation::kMultiply;
  std::function<void(double)> backward = [lhs, rhs](double out_grad) -> void {
    rhs.GradRef() += lhs * out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto operator*(const Value& lhs, double rhs) -> Value {
  return rhs * lhs;
}

auto operator-(double lhs, const Value& rhs) -> Value {
  Value out(lhs - rhs.Data(), {rhs});
  out.m_state_->op_ = Operation::kSubtract;
  std::function<void(double)> backward = [rhs](double out_grad) -> void {
    rhs.GradRef() += -1 * out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto operator-(const Value& lhs, double rhs) -> Value {
  Value out(lhs.Data() - rhs, {lhs});
  out.m_state_->op_ = Operation::kSubtract;
  std::function<void(double)> backward = [lhs](double out_grad) -> void {
    lhs.GradRef() += out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto operator+(double lhs, const Value& rhs) -> Value {
  Value out(lhs + rhs.Data(), {rhs});
  out.m_state_->op_ = Operation::kAdd;
  std::function<void(double)> backward = [rhs](double out_grad) -> void {
    rhs.GradRef() += out_grad;
  };
  out.SetBackward(backward);
  return out;
}

auto operator+(const Value& lhs, double rhs) -> Value {
  return rhs + lhs;
}
