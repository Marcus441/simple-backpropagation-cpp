#include "util/graphing.hpp"

#include <format>
#include <fstream>
#include <set>

namespace util::graphing {
void BuildDot(const Value& v, std::set<const void*>& visited, std::ofstream& out) {
  const void* id = v.Id();
  if (!visited.insert(id).second)
    return;

  out << std::format("\tnode_{} [label=\"{{ {} | data {:.2f} | grad {:.2f}}}\"];\n", v.Id(),
                     v.Label(), v.Data(), v.Grad());

  if (v.Op() != Operation::kNone) {
    std::string label;
    if (v.Op() == Operation::kAdd)
      label = "+";
    if (v.Op() == Operation::kSubtract)
      label = "-";
    if (v.Op() == Operation::kMultiply)
      label = "*";
    if (v.Op() == Operation::kDivide)
      label = "/";
    if (v.Op() == Operation::kExp)
      label = "^e";
    if (v.Op() == Operation::kPower)
      label = "^";
    if (v.Op() == Operation::kTanh)
      label = "tanh";

    out << std::format("\top_{0} [label=\"{1}\", shape=circle];\n", id, label);
    out << std::format("\top_{0} -> node_{0};\n", id);
  }

  for (const auto& child : v.Prev()) {
    BuildDot(child, visited, out);
    if (v.Op() != Operation::kNone) {
      out << std::format("\tnode_{} -> op_{};\n", child.Id(), id);
    } else {
      out << std::format("\tnode_{} -> node_{};\n", child.Id(), id);
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
