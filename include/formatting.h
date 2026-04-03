#pragma once

#include "value.h"
#include <format>

template <> struct std::formatter<Value> : std::formatter<double> {
  auto format(const Value &value, std::format_context &ctx) const {
    return std::formatter<double>::format(value.Data(), ctx);
  }
};
