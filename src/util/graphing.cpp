#include "util/graphing.h"
#include <format>
#include <fstream>
#include <set>

namespace util::graphing {
void build_dot(const Value &v, std::set<const void *> &visited,
               std::ofstream &out) {

  const void *id = v.id();
  if (!visited.insert(id).second)
    return;

  out << std::format(
      "\tnode_{} [label=\"{{ {} | data {:.2f} | grad {:.2f}}}\"];\n", v.id(),
      v.label(), v.data(), v.grad());

  if (v.op() != Operation::NONE) {
    std::string label;
    if (v.op() == Operation::ADD)
      label = "+";
    if (v.op() == Operation::MULTIPLY)
      label = "*";
    if (v.op() == Operation::TANH)
      label = "tanh";

    out << std::format("\top_{0} [label=\"{1}\", shape=circle];\n", id, label);
    out << std::format("\top_{0} -> node_{0};\n", id);
  }

  for (const auto &child : v.prev()) {
    build_dot(child, visited, out);
    if (v.op() != Operation::NONE) {
      out << std::format("\tnode_{} -> op_{};\n", child.id(), id);
    }
  }
}

void export_to_dot(const Value &root, const std::string &filename) {
  std::ofstream outfile(filename);
  std::set<const void *> visited;

  outfile << "digraph G {\n";
  outfile << "\trankdir=\"LR\";\n";
  outfile << "\tnode [shape=record];\n";
  build_dot(root, visited, outfile);
  outfile << '}';
  outfile.close();
}
} // namespace util::graphing
