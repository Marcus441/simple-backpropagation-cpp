#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

enum class Operation {
  kAdd,
  kMultiply,
  kTanh,
  kNone,
};

class Value {
 public:
  // Base constructor
  Value(double data) : m_state_(std::make_shared<State>(data)) {}

  // Graph constructor
  Value(double data, std::initializer_list<Value> children)
      : m_state_(std::make_shared<State>(data, children)) {}

  friend Value operator+(const Value& lhs, const Value& rhs);
  friend Value operator*(const Value& lhs, const Value& rhs);

  // Accessors
  const std::string& Label() const { return m_state_->label_; }
  double Data() const { return m_state_->data_; }
  double Grad() const { return m_state_->grad_; }
  std::vector<Value> Prev() const { return m_state_->prev_; }
  Operation Op() const { return m_state_->op_; }
  const void* Id() const { return m_state_.get(); }
  void Backward();

  // Setters
  void Label(std::string label) { m_state_->label_ = std::move(label); }
  void Grad(double grad) const { m_state_->grad_ += grad; }

  // Math
  Value Tanh();

  void ClearGraph() {
    m_state_->backward_ = nullptr;
    m_state_->prev_.clear();
  }

 private:
  struct State {
    double grad_{0.0};
    double data_;
    std::vector<Value> prev_;
    State(double d, std::vector<Value> p = {}) : data_(d), prev_(p) {};
    Operation op_ = Operation::kNone;
    std::string label_;
    std::function<void(double)> backward_ = [](double) {};
  };
  std::shared_ptr<State> m_state_;
  void SetBackward(std::function<void(double)> backward) {
    m_state_->backward_ = std::move(backward);
  }
};

// Export formatting rules
#include "formatting.h"  // IWYU pragma: keep
