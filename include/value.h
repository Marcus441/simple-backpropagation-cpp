#pragma once

#include <initializer_list>
#include <memory>
#include <ostream>
#include <vector>

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
  double data() const { return m_state->data; }
  std::vector<Value> prev() const { return m_state->prev; }

private:
  struct State {
    double data;
    std::vector<Value> prev;
    State(double d, std::vector<Value> p = {}) : data(d), prev(p) {};
  };
  std::shared_ptr<State> m_state;
};
// Export formatting rules
#include "formatting.h" // IWYU pragma: keep
