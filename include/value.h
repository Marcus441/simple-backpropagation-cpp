#pragma once

#include <initializer_list>
#include <memory>
#include <string>
#include <vector>

#define MAKE_VALUE(name, val)                                                  \
  Value name{val};                                                             \
  name.label(#name);

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
  const std::string &label() const { return m_label; }
  double data() const { return m_state->data; }
  double grad() const { return m_state->grad; }
  std::vector<Value> prev() const { return m_state->prev; }
  Operation op() const { return m_op; }
  const void *id() const { return m_state.get(); }

  // Setters
  void label(std::string label) { m_label = std::move(label); }

  // Math
  Value tanh();

private:
  struct State {
    double grad{0.0};
    double data;
    std::vector<Value> prev;
    State(double d, std::vector<Value> p = {}) : data(d), prev(p) {};
  };
  std::shared_ptr<State> m_state;
  Operation m_op = Operation::NONE;
  std::string m_label;
};

// Export formatting rules
#include "formatting.h" // IWYU pragma: keep
