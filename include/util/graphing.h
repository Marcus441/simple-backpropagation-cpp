#pragma once
#include "value.h"
#include <string>

namespace util::graphing {

void export_to_dot(const Value &root, const std::string &filename);

} // namespace util::graphing
