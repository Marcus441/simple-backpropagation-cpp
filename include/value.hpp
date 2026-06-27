#pragma once

#include <cstdint>
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <utility>
#include <vector>

enum class Operation {
  kAdd,
  kSubtract,
  kMultiply,
  kDivide,
  kTanh,
  kExp,
  kPower,
  kNone,
};

class Value {
 public:
  Value() : Value(0.0) {}
  // Base constructor
  explicit Value(double data) : m_state_(std::make_shared<State>(data)) {}

  // Graph constructor
  Value(double data, std::initializer_list<Value> children)
      : m_state_(std::make_shared<State>(data, children)) {}

  // Addition
  friend Value operator+(const Value& lhs, const Value& rhs);
  friend Value operator+(double lhs, const Value& rhs);
  friend Value operator+(const Value& lhs, double rhs);

  // Subtraction
  friend Value operator-(const Value& lhs, const Value& rhs);
  friend Value operator-(double lhs, const Value& rhs);
  friend Value operator-(const Value& lhs, double rhs);

  // Multiplication
  friend Value operator*(const Value& lhs, const Value& rhs);
  friend Value operator*(double lhs, const Value& rhs);
  friend Value operator*(const Value& lhs, double rhs);

  // Division
  friend Value operator/(const Value& lhs, const Value& rhs);
  friend Value operator/(const Value& lhs, double rhs);
  friend Value operator/(double lhs, const Value& rhs);

  // Accessors
  [[nodiscard]] const std::string& Label() const { return m_state_->label_; }
  [[nodiscard]] double Data() const { return m_state_->data_; }
  [[nodiscard]] double Grad() const { return m_state_->grad_; }
  [[nodiscard]] std::vector<Value> Prev() const { return m_state_->prev_; }
  [[nodiscard]] Operation Op() const { return m_state_->op_; }
  [[nodiscard]] const void* Id() const { return m_state_.get(); }
  void Backward();

  // Setters
  void Label(std::string label) { m_state_->label_ = std::move(label); }
  void SetData(double d) { m_state_->data_ = d; }
  void ZeroGrad() { m_state_->grad_ = 0; }

  // Math
  Value Tanh();
  Value Exp();
  [[nodiscard]] Value Pow(double other) const;

  void ClearGraph() {
    m_state_->backward_ = nullptr;
    m_state_->prev_.clear();
  }

 private:
  [[nodiscard]] double& GradRef() const { return m_state_->grad_; }
  struct State {
    double grad_{0.0};
    double data_;
    std::vector<Value> prev_;
    State(double d, std::vector<Value> p = {}) : data_(d), prev_(std::move(p)) {};
    Operation op_ = Operation::kNone;
    std::string label_;
    std::function<void(double)> backward_ = [](double) -> void {};
  };
  std::shared_ptr<State> m_state_;
  void SetBackward(std::function<void(double)> backward) {
    m_state_->backward_ = std::move(backward);
  }
};

// Export formatting rules
#include "formatting.hpp"  // IWYU pragma: keep
