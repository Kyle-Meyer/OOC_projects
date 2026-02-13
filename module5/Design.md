# Module 5 — Probability Class Design Document

## Overview

A C++ class `Probability` that models the probability of independent events. A probability value ranges from `0.0` (impossible) to `1.0` (certain). The class overloads bitwise and arithmetic operators to express combinations of independent probabilities using intuitive notation.

---

## Class: `Probability`

### Data Members

| Member | Type | Description |
|--------|------|-------------|
| `p_`   | `double` | The probability value, invariant: `0.0 <= p_ <= 1.0` |

### Invariant Enforcement

Values provided via construction or assignment are **clamped** to the valid range `[0.0, 1.0]`:
- Values less than `0.0` are set to `0.0`.
- Values greater than `1.0` are set to `1.0`.

This provides defined behavior for invalid inputs without throwing exceptions.

---

## Construction & Assignment

| Signature | Description |
|-----------|-------------|
| `Probability()` | Default constructor; initializes to `0.0` |
| `Probability(double val)` | Constructs from a `double`; clamps to `[0.0, 1.0]` |
| `Probability& operator=(double val)` | Assigns from a `double`; clamps to `[0.0, 1.0]` |

The `double` constructor is intentionally **not** `explicit`, allowing implicit conversion from `double` to `Probability`. This keeps expressions like `Probability p = 0.5;` natural.

---

## Operators

All binary operators follow the canonical form from *C++ Coding Standards* Rule #27:
1. Define compound-assignment operators (`&=`, `|=`, `^=`, `-=`) as **member functions** that return `*this` by reference.
2. Define the corresponding binary operators (`&`, `|`, `^`, `-`) as **non-member functions** implemented in terms of the compound-assignment operators, taking the left-hand side **by value**.

### Probability Math (Independent Events)

| Expression | Meaning | Formula |
|------------|---------|---------|
| `A & B`   | Both A and B occur | `A.p_ * B.p_` |
| `A \| B`  | Either A or B (or both) occur | `A.p_ + B.p_ - A.p_ * B.p_` |
| `A ^ B`   | Either A or B but not both (exclusive or) | `A.p_ + B.p_ - 2 * A.p_ * B.p_` |
| `A - B`   | A occurs but B does not | `A.p_ * (1.0 - B.p_)` |
| `~A`       | A does not occur (complement) | `1.0 - A.p_` |

### Operator Summary

#### Member Functions

| Signature | Notes |
|-----------|-------|
| `Probability& operator&=(const Probability& rhs)` | Compound AND-assign |
| `Probability& operator\|=(const Probability& rhs)` | Compound OR-assign |
| `Probability& operator^=(const Probability& rhs)` | Compound XOR-assign |
| `Probability& operator-=(const Probability& rhs)` | Compound difference-assign |
| `Probability operator~() const` | Unary complement (member) |

#### Non-Member Functions (Canonical Binary Operators)

Each takes the LHS **by value** so the compound-assignment can operate on the copy:

```cpp
Probability operator&(Probability lhs, const Probability& rhs);
Probability operator|(Probability lhs, const Probability& rhs);
Probability operator^(Probability lhs, const Probability& rhs);
Probability operator-(Probability lhs, const Probability& rhs);
```

### Accessor

| Signature | Description |
|-----------|-------------|
| `double value() const` | Returns the stored probability |

### Comparison (optional but useful)

| Signature | Description |
|-----------|-------------|
| `bool operator==(const Probability& rhs) const` | Equality via `==` defaulted or custom with epsilon |

### Stream Output (optional but useful)

| Signature | Description |
|-----------|-------------|
| `friend std::ostream& operator<<(std::ostream&, const Probability&)` | Prints the probability value |

---

## File Layout

```
module5/
├── inc/
│   └── Probability.h      // Class declaration, non-member operator declarations
├── src/
│   ├── Probability.cpp     // Member & non-member operator definitions
│   └── main.cpp            // Driver / test program
└── Design.md               // This document
```

### `inc/Probability.h` — Header

```
#pragma once
#include <iostream>

class Probability {
public:
    Probability();
    Probability(double val);
    Probability& operator=(double val);

    double value() const;

    Probability& operator&=(const Probability& rhs);
    Probability& operator|=(const Probability& rhs);
    Probability& operator^=(const Probability& rhs);
    Probability& operator-=(const Probability& rhs);
    Probability  operator~() const;

    friend std::ostream& operator<<(std::ostream& os, const Probability& p);

private:
    double p_;
    static double clamp(double v);
};

// Non-member binary operators (canonical form)
Probability operator&(Probability lhs, const Probability& rhs);
Probability operator|(Probability lhs, const Probability& rhs);
Probability operator^(Probability lhs, const Probability& rhs);
Probability operator-(Probability lhs, const Probability& rhs);
```

### `src/Probability.cpp` — Implementation

- `clamp(double v)`: returns `std::max(0.0, std::min(1.0, v))`.
- Constructors/assignment call `clamp` on the incoming value.
- Compound-assignment operators apply the formulas from the table above, then clamp the result (although mathematically the formulas already stay in range for valid inputs, clamping guards against floating-point drift).
- Non-member binary operators follow the pattern:
  ```cpp
  Probability operator&(Probability lhs, const Probability& rhs) {
      lhs &= rhs;
      return lhs;
  }
  ```

### `src/main.cpp` — Driver

Demonstrates:
1. Construction and assignment from `double` (valid and invalid values).
2. Each operator with sample probabilities and printed results.
3. Compound expressions such as `(A | B) & ~C`.

---

## Example Usage

```cpp
Probability A(0.5);
Probability B(0.3);

Probability both    = A & B;    // 0.15
Probability either  = A | B;    // 0.65
Probability xorProb = A ^ B;    // 0.50
Probability aNotB   = A - B;    // 0.35
Probability notA    = ~A;       // 0.50

Probability bad(-0.5);          // clamped to 0.0
Probability bad2(1.5);          // clamped to 1.0
```

---

## Design Rationale

1. **Clamping vs. Throwing**: Clamping provides defined, predictable behavior without requiring callers to handle exceptions. This matches the assignment requirement of "defined behavior if the value is not valid."

2. **Non-member binary operators (Rule #27)**: Defining binary operators as non-members in terms of compound-assignment operators ensures symmetry (both sides can convert from `double`) and avoids code duplication.

3. **Implicit `double` conversion**: The non-explicit constructor allows `Probability p = 0.5;` and mixed expressions like `A & 0.3` naturally, since `0.3` implicitly converts to `Probability`.

4. **Single `double` member**: Keeps the class lightweight with value semantics. The compiler-generated copy/move constructors and assignment operators are correct and sufficient.
