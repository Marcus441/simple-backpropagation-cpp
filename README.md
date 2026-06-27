# micrograd-cpp

A C++23 implementation of [Andrej Karpathy's micrograd](https://github.com/karpathy/micrograd) — a scalar-valued automatic differentiation engine with a neural network library built on top of it.

## Overview

The core of this project is a `Value` class that wraps a scalar `double` and records the computation graph as operations are performed. Calling `Backward()` on any node in the graph propagates gradients back through it via reverse-mode automatic differentiation (backpropagation).

On top of the autograd engine, a minimal neural network API is provided: `Neuron`, `Layer`, and `MLP`.

## Architecture

```
Value          — scalar node in a computation graph; supports +, -, *, /, tanh, exp, pow
Neuron         — single neuron: dot(w, x) + b passed through tanh
Layer          — a row of neurons applied in parallel to the same input
MLP            — a stack of layers; takes a vector<double> and produces a single Value
util::graphing — exports a Value's computation graph to Graphviz DOT format
```

### Value

`Value` is the fundamental building block. It stores:

- `data` — the scalar value
- `grad` — the accumulated gradient (populated by `Backward()`)
- `prev` — child nodes in the computation graph
- `op` — the operation that produced this node
- `backward_` — closure that propagates gradient to children

Operations are overloaded so expressions like `(a * b + c).Tanh()` automatically build the graph.

```cpp
Value a{2.0};
Value b{3.0};
Value c = (a * b).Tanh();
c.Backward();
// a.Grad() and b.Grad() are now populated
```

### Neuron

A single neuron with `n_inputs` weights and one bias, all initialised from `Uniform(-1, 1)`. Forward pass computes `tanh(w · x + b)`.

### Layer

A `Layer` holds `n_out` neurons each expecting `n_in` inputs. Its call operator returns a `vector<Value>` — one activated output per neuron.

### MLP

An `MLP` is constructed from an `initializer_list<LayerSize>` that describes each layer's input and output width. The call operator forwards a `vector<double>` through every layer and returns the single scalar output of the final neuron.

```cpp
MLP net({{.n_in_ = 3, .n_out_ = 4},
         {.n_in_ = 4, .n_out_ = 4},
         {.n_in_ = 4, .n_out_ = 1}});

Value pred = net({2.0, 3.0, -1.0});
```

### Computation Graph Export

`util::graphing::ExportToDot(root, "file.dot")` writes the full computation graph rooted at `root` to a Graphviz DOT file, which can be rendered with:

```sh
dot -Tsvg file.dot -o graph.svg
```

## Requirements

| Tool | Version |
|------|---------|
| Clang / libc++ | 18+ recommended |
| CMake | 3.31+ |
| Ninja | any recent |

The project uses **C++23** (`std::print`, etc.) and requires libc++ or modern libstdc++.

## Building

### With Nix (recommended)

A `flake.nix` is provided that supplies a complete dev shell with Clang, CMake, Ninja, LLDB, GDB, and Graphviz.

```sh
nix develop        # enter the dev shell
cmake --preset default
cmake --build build --preset debug
./build/Debug/back-prop
```

### Without Nix

Ensure `clang++` with libc++ and CMake 3.31+ are on your `PATH`, then:

```sh
cmake --preset default
cmake --build build --preset debug
./build/Debug/back-prop
```

## Build Presets

| Preset | Description |
|--------|-------------|
| `debug` | Debug build (`-g -Og`) |
| `release` | Optimised build (`-O3`) |
| `relwithdebinfo` | Optimised with debug info (`-O2 -g`) |

```sh
cmake --build build --preset release
cmake --build build --preset debug
```

## Demo

`src/main.cpp` demonstrates a full training step on a toy dataset: four 3-dimensional inputs with binary targets `{1, -1, -1, 1}`, MSE loss computed over one forward pass, and a single call to `Backward()` that fills gradients throughout the network.

```sh
./build/Debug/back-prop
# loss before backward 0.xx
# loss after backward  0.xx  (unchanged; backward only fills .Grad())
dot -Tsvg file.dot -o graph.svg   # visualise the computation graph
```

## Project Structure

```
.
+-- include/
|   +-- value.hpp          # Value class and operator declarations
|   +-- neuron.hpp         # Neuron class
|   +-- layer.hpp          # Layer class and LayerSize struct
|   +-- MLP.hpp            # MLP class
|   +-- formatting.hpp     # std::formatter specialisation for Value
|   +-- util/
|       +-- graphing.hpp   # ExportToDot declaration
+-- src/
|   +-- main.cpp           # Demo: forward pass + backward + dot export
|   +-- value.cpp          # Value operator and backward implementations
|   +-- neuron.cpp         # Neuron implementation
|   +-- layer.cpp          # Layer implementation
|   +-- MLP.cpp            # MLP implementation
|   +-- util/
|       +-- graphing.cpp   # DOT export implementation
+-- CMakeLists.txt
+-- CMakePresets.json
+-- flake.nix
```

## References

- [micrograd](https://github.com/karpathy/micrograd) by Andrej Karpathy
- [The spelled-out intro to neural networks and backpropagation](https://www.youtube.com/watch?v=VMj-3S1tku0) (YouTube)
