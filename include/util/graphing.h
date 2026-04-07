#pragma once
#include <string>

#include "value.h"

namespace util::graphing {

void ExportToDot(const Value& root, const std::string& filename);

}  // namespace util::graphing
