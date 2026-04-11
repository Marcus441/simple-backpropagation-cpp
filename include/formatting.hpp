#pragma once

#include <format>

#include "value.hpp"

template <>
struct std::formatter<Value> : std::formatter<double> {
  auto format(const Value& value, std::format_context& ctx) const {
    return std::formatter<double>::format(value.Data(), ctx);
  }
};
