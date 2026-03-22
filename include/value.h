#pragma once

#include <ostream>

class Value {
public:
  Value(double data) : data(data) {}
  Value(Value &&) = default;
  Value(const Value &) = default;
  Value &operator=(Value &&) = default;
  Value &operator=(const Value &) = default;

  friend Value operator+(const Value &lhs, const Value &rhs);
  friend Value operator*(const Value &lhs, const Value &rhs);
  friend std::ostream &operator<<(std::ostream &os, const Value &v);

private:
  double data;
};
