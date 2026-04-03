#pragma once
#include "value.h"
#include <string>

namespace util::graphing {

void ExportToDot(const Value &root, const std::string &filename);

} // namespace util::graphing
