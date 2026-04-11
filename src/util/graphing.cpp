#include "util/graphing.hpp"

#include <format>
#include <fstream>
#include <print>
#include <set>

namespace util::graphing {
void BuildDot(const Value& v, std::set<const void*>& visited, std::ofstream& out) {
  const void* id = v.Id();
  if (!visited.insert(id).second) {
    return;
  }

  std::println(out, "\tnode_{} [label=\"{{ {} | data {:.2f} | grad {:.2f}}}\"];", v.Id(), v.Label(),
               v.Data(), v.Grad());

  if (v.Op() != Operation::kNone) {
    std::string label;
    // clang-format off
    switch (v.Op()) {
      case Operation::kAdd:      label = "+";    break;
      case Operation::kSubtract: label = "-";    break;
      case Operation::kMultiply: label = "*";    break;
      case Operation::kDivide:   label = "/";    break;
      case Operation::kExp:      label = "^e";   break;
      case Operation::kPower:    label = "^";    break;
      case Operation::kTanh:     label = "tanh"; break;
      case Operation::kNone:     break;
    }
    // clang-format on
    std::println(out, "\top_{0} [label=\"{1}\", shape=circle];", id, label);
    std::println(out, "\top_{0} -> node_{0};", id);
  }

  for (const auto& child : v.Prev()) {
    BuildDot(child, visited, out);
    if (v.Op() != Operation::kNone) {
      std::println(out, "\tnode_{} -> op_{};", child.Id(), id);
    } else {
      std::println(out, "\tnode_{} -> node_{};", child.Id(), id);
    }
  }
}

void ExportToDot(const Value& root, const std::string& filename) {
  std::ofstream outfile(filename);
  std::set<const void*> visited;

  outfile << "digraph G {\n";
  outfile << "\trankdir=\"LR\";\n";
  outfile << "\tnode [shape=record];\n";
  BuildDot(root, visited, outfile);
  outfile << '}';
  outfile.close();
}
}  // namespace util::graphing
