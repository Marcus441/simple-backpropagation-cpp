#include <cstddef>
#include <memory>
#include <print>
#include <vector>

#include "MLP.hpp"
#include "util/graphing.hpp"
#include "value.hpp"

std::vector<Value> ForwardPass(MLP& mlp, const std::vector<std::vector<double>>& xs) {
  std::vector<Value> preds;
  preds.reserve(xs.size());
  for (const auto& x : xs) {
    preds.emplace_back(mlp(x));
  }
  return preds;
}

Value ComputeLoss(const std::vector<Value>& preds, const std::vector<double>& y) {
  Value loss = (preds[0] - y[0]).Pow(2);
  for (size_t i = 1; i < preds.size(); ++i) {
    loss = loss + (preds[i] - y[i]).Pow(2);
  }
  return loss / static_cast<double>(preds.size());
}

void Step(MLP& mlp, double learning_rate) {
  auto params = mlp.Parameters().lock();
  for (auto& p : *params) {
    p.SetData(p.Data() - (learning_rate * p.Grad()));
  }
  for (auto& p : *params) {
    p.ZeroGrad();
  }
}

auto main() -> int {
  MLP mlp({{.n_in_ = 3, .n_out_ = 4}, {.n_in_ = 4, .n_out_ = 4}, {.n_in_ = 4, .n_out_ = 1}});

  std::vector<std::vector<double>> xs{
      {2.0, 3.0, -1.0}, {3.0, -1.0, 0.5}, {0.5, 1.0, 1.0}, {1.0, 1.0, -1.0}};
  std::vector<double> y{1.0, -1.0, -1.0, 1.0};

  double learning_rate = 0.1;
  int steps = 100;

  for (int s = 0; s < steps; ++s) {
    auto preds = ForwardPass(mlp, xs);
    auto loss = ComputeLoss(preds, y);
    loss.Backward();
    std::println("step {:3} | loss {:.10f}", s, loss.Data());

    if (s == 0) {
      util::graphing::ExportToDot(loss, "before_gradient_descent.dot");
    }

    Step(mlp, learning_rate);
  }

  auto final_preds = ForwardPass(mlp, xs);
  auto final_loss = ComputeLoss(final_preds, y);
  final_loss.Backward();
  util::graphing::ExportToDot(final_loss, "after_gradient_descent.dot");
}
