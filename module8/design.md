# Module 8 — Expression Tree Design

## Overview

Build a C++ class hierarchy representing arithmetic expression trees. Interior nodes are
operations; leaf nodes are constants or variables. The tree supports printing, evaluation,
and symbolic differentiation.

---

## Type Aliases (in `inc/Node.h`)

```cpp
#include <memory>
#include <map>
#include <string>

class Node;
using node_ptr      = std::shared_ptr<Node>;
using SymbolTable   = std::map<std::string, double>;
using symbol_table_ptr = std::shared_ptr<SymbolTable>;
```

---

## Class Hierarchy

```
Node  (abstract)
├── Constant
├── Variable
├── UnaryMinus
└── BinaryOp  (abstract)
    ├── Add
    ├── Sub
    ├── Mul
    └── Div
```

---

## Class Designs

### `Node` — abstract base (`inc/Node.h`)

```cpp
class Node {
public:
    virtual ~Node() = default;
    virtual double   evaluate()                          const = 0;
    virtual node_ptr derivative(const std::string& var)  const = 0;
    virtual void     print(std::ostream& os)             const = 0;
};

// Free function so `std::cout << node_ptr` works
inline std::ostream& operator<<(std::ostream& os, const node_ptr& n) {
    n->print(os);
    return os;
}
```

---

### `Constant` — leaf node (`inc/Constant.h`)

Stores a `double` value.

| Method | Behaviour |
|---|---|
| `evaluate()` | returns stored value |
| `derivative(x)` | `make_shared<Constant>(0.0)` |
| `print(os)` | outputs `(value)` |

---

### `Variable` — leaf node (`inc/Variable.h`)

Stores a `string` name **and a `symbol_table_ptr`**.
The value is **never stored in the node** — it is always looked up at evaluation time.

```cpp
Variable(const std::string& name, symbol_table_ptr table);
```

| Method | Behaviour |
|---|---|
| `evaluate()` | `(*table_)[name_]` |
| `derivative(x)` | `name_ == x` → `Constant(1.0)`, else `Constant(0.0)` |
| `print(os)` | outputs `(name)` |

All `Variable` nodes that represent the same symbol must share the **same**
`symbol_table_ptr` so that updating one entry is immediately visible everywhere.

---

### `BinaryOp` — abstract intermediate (`inc/BinaryOp.h`)

```cpp
class BinaryOp : public Node {
protected:
    node_ptr left_, right_;
    BinaryOp(node_ptr l, node_ptr r) : left_(l), right_(r) {}
};
```

Concrete subclasses only need to implement `evaluate()`, `derivative()`, and `print()`.

---

### `Add` : `BinaryOp` (`inc/Add.h`)

| Method | Behaviour |
|---|---|
| `evaluate()` | `left_->evaluate() + right_->evaluate()` |
| `derivative(x)` | `Add(left_->derivative(x), right_->derivative(x))` |
| `print(os)` | `((left)+(right))` |

---

### `Sub` : `BinaryOp` (`inc/Sub.h`)

| Method | Behaviour |
|---|---|
| `evaluate()` | `left_->evaluate() - right_->evaluate()` |
| `derivative(x)` | `Sub(left_->derivative(x), right_->derivative(x))` |
| `print(os)` | `((left)-(right))` |

---

### `Mul` : `BinaryOp` (`inc/Mul.h`)

Product rule: **d(u·v) = u·dv + v·du**

> `left_` and `right_` are **shared** into the derivative tree (not copied).
> This is why `shared_ptr` is required — both the original tree and the
> derivative tree hold references to the same subtrees.

| Method | Behaviour |
|---|---|
| `evaluate()` | `left_->evaluate() * right_->evaluate()` |
| `derivative(x)` | `Add( Mul(left_, right_->derivative(x)),`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`Mul(right_, left_->derivative(x)) )` |
| `print(os)` | `((left)*(right))` |

---

### `Div` : `BinaryOp` (`inc/Div.h`)

Quotient rule: **d(u/v) = (v·du − u·dv) / (v·v)**

| Method | Behaviour |
|---|---|
| `evaluate()` | `left_->evaluate() / right_->evaluate()` |
| `derivative(x)` | `Div( Sub( Mul(right_, left_->derivative(x)),`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`Mul(left_, right_->derivative(x)) ),`<br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`Mul(right_, right_) )` |
| `print(os)` | `((left)/(right))` |

---

### `UnaryMinus` : `Node` (`inc/UnaryMinus.h`)

Handles the `-u` rule from the derivative table.

```cpp
UnaryMinus(node_ptr child);
```

| Method | Behaviour |
|---|---|
| `evaluate()` | `-child_->evaluate()` |
| `derivative(x)` | `UnaryMinus(child_->derivative(x))` |
| `print(os)` | `(-(child))` |

---

## Symbol Table

The symbol table is created once in `main.cpp` and passed to every `Variable` at
construction time:

```cpp
auto table = std::make_shared<SymbolTable>();
(*table)["Xray"]   = 2.0;
(*table)["Yellow"] = 3.0;
(*table)["Zebra"]  = 5.0;
```

Variable nodes only hold a `symbol_table_ptr`; they never cache the double value.

---

## Constructing the Example Tree

```cpp
// Convenience: aliases to shorten make_shared calls
auto C = [](double v)                      { return std::make_shared<Constant>(v); };
auto V = [&](const std::string& n)         { return std::make_shared<Variable>(n, table); };

node_ptr tree = std::make_shared<Add>(
    std::make_shared<Mul>(C(2.3), V("Xray")),
    std::make_shared<Mul>(V("Yellow"),
        std::make_shared<Sub>(V("Zebra"), V("Xray")))
);
```

---

## `main.cpp` Checklist

1. Create the shared symbol table and populate it.
2. Construct the expression tree with `make_shared`.
3. Print the tree: `std::cout << tree;`
4. Evaluate: `double d = tree->evaluate();` → expect `13.6`
5. Compute derivative w.r.t. `"Xray"`: `node_ptr dx = tree->derivative("Xray");`
6. Print `dx` and evaluate it → expect `-0.7`

---

## File Layout

```
module8/
├── CMakeLists.txt
├── inc/
│   ├── Node.h          ← node_ptr, SymbolTable typedefs, operator<<
│   ├── Constant.h
│   ├── Variable.h
│   ├── BinaryOp.h
│   ├── Add.h
│   ├── Sub.h
│   ├── Mul.h
│   ├── Div.h
│   └── UnaryMinus.h
└── src/
    └── main.cpp
```

All class implementations are **inline inside their headers** (these classes are small
and header-only avoids separate compilation overhead for a project this size).

---

## `CMakeLists.txt`

```cmake
cmake_minimum_required(VERSION 3.10)
project(ExpressionTree)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(inc)

add_executable(expression_tree src/main.cpp)
```

---

## Key C++ Notes

- **`std::shared_ptr` is mandatory** because derivative trees share interior subtrees
  with the original tree. Raw pointers would cause double-free or dangling pointer bugs.
- Use `std::make_shared<T>(...)` everywhere — never raw `new`.
- `evaluate()` and `print()` are `const` — they do not modify the tree.
- `derivative()` is `const` and returns a **new** tree; the original tree is unchanged.
- The symbol table is mutated externally; evaluating the same tree with different table
  values produces different results without rebuilding the tree.
