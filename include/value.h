#pragma once

#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

enum class Operation {
  ADD,
  MULTIPLY,
  TANH,
  NONE,
};

class Value {
public:
  // Base constructor
  Value(double data) : m_state(std::make_shared<State>(data)) {}

  // Graph constructor
  Value(double data, std::initializer_list<Value> children)
      : m_state(std::make_shared<State>(data, children)) {}

  friend Value operator+(const Value &lhs, const Value &rhs);
  friend Value operator*(const Value &lhs, const Value &rhs);

  // Accessors
  const std::string &label() const { return m_state->label; }
  double data() const { return m_state->data; }
  double grad() const { return m_state->grad; }
  std::vector<Value> prev() const { return m_state->prev; }
  Operation op() const { return m_state->op; }
  const void *id() const { return m_state.get(); }
  void backward();

  // Setters
  void label(std::string label) { m_state->label = std::move(label); }
  void grad(double grad) const { m_state->grad += grad; }

  // Math
  Value tanh();

private:
  struct State {
    double grad{0.0};
    double data;
    std::vector<Value> prev;
    State(double d, std::vector<Value> p = {}) : data(d), prev(p) {};
    Operation op = Operation::NONE;
    std::string label;
    std::function<void()> backward = [] {};
  };
  std::shared_ptr<State> m_state;
  void set_backward(std::function<void()> backward) {
    m_state->backward = std::move(backward);
  }
};

// Export formatting rules
#include "formatting.h" // IWYU pragma: keep
