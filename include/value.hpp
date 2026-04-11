#pragma once

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
  // Base constructor
  Value(double data) : m_state_(std::make_shared<State>(data)) {}

  // Graph constructor
  Value(double data, std::initializer_list<Value> children)
      : m_state_(std::make_shared<State>(data, children)) {}

  // Addition
  friend auto operator+(const Value& lhs, const Value& rhs) -> Value;
  friend auto operator+(double lhs, const Value& rhs) -> Value;
  friend auto operator+(const Value& lhs, double rhs) -> Value;

  // Subtraction
  friend auto operator-(const Value& lhs, const Value& rhs) -> Value;
  friend auto operator-(double lhs, const Value& rhs) -> Value;
  friend auto operator-(const Value& lhs, double rhs) -> Value;

  // Multiplication
  friend auto operator*(const Value& lhs, const Value& rhs) -> Value;
  friend auto operator*(double lhs, const Value& rhs) -> Value;
  friend auto operator*(const Value& lhs, double rhs) -> Value;

  // Division
  friend auto operator/(const Value& lhs, const Value& rhs) -> Value;

  // Accessors
  [[nodiscard]] auto Label() const -> const std::string& { return m_state_->label_; }
  [[nodiscard]] auto Data() const -> double { return m_state_->data_; }
  [[nodiscard]] auto Grad() const -> double { return m_state_->grad_; }
  [[nodiscard]] auto Prev() const -> std::vector<Value> { return m_state_->prev_; }
  [[nodiscard]] auto Op() const -> Operation { return m_state_->op_; }
  [[nodiscard]] auto Id() const -> const void* { return m_state_.get(); }
  void Backward();

  // Setters
  void Label(std::string label) { m_state_->label_ = std::move(label); }

  // Math
  auto Tanh() -> Value;
  auto Exp() -> Value;
  [[nodiscard]] auto Pow(double other) const -> Value;

  void ClearGraph() {
    m_state_->backward_ = nullptr;
    m_state_->prev_.clear();
  }

 private:
  [[nodiscard]] auto GradRef() const -> double& { return m_state_->grad_; }
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
