# Design: Matrix Addition — Threaded vs Unthreaded

## Goal
Compare execution time of matrix addition in two implementations:
1. **Unthreaded** — single-threaded, sequential row processing
2. **Threaded** — multi-threaded, rows divided across N threads

---

## Shared Constants

```cpp
#define NUM_ROWS 1000
#define NUM_COLS 1000
#define NUM_THREADS 4
```

Matrices will be populated with random doubles before timing begins.

---

## Core Algorithm (provided)

```cpp
void matrixAdd(double leftMatrix[NUM_ROWS][NUM_COLS],
               double rightMatrix[NUM_ROWS][NUM_COLS],
               double resultMatrix[NUM_ROWS][NUM_COLS],
               int startRow,
               int endRow,
               double &sum);
```

Computes `resultMatrix[row][col] = left + right` for rows `[startRow, endRow]` and accumulates the element sum into `sum`.

---

## Implementation 1: Unthreaded

- Call `matrixAdd` once with `startRow=0`, `endRow=NUM_ROWS-1`
- Wrap with `chrono::high_resolution_clock` to capture elapsed time
- Print total sum and elapsed time

---

## Implementation 2: Threaded

- Divide rows evenly across `NUM_THREADS` threads
  - Thread `i` handles rows `[i * (NUM_ROWS/NUM_THREADS), (i+1) * (NUM_ROWS/NUM_COLS) - 1]`
- Each thread gets its own `sum` variable to avoid race conditions
- Use `std::thread` with a lambda or struct to pass args (since `matrixAdd` takes a reference param)
- After `join()` all threads, accumulate per-thread sums into a final total
- Wrap thread creation through join with `chrono` for elapsed time

---

## File Structure

```
module14/
  assigment.md
  design.md
  matrix_add.h       # NUM_ROWS, NUM_COLS, NUM_THREADS, matrixAdd()
  unthreaded.cpp     # single-threaded driver + timing
  threaded.cpp       # multi-threaded driver + timing
  Makefile           # builds both targets: unthreaded, threaded
```

---

## Timing & Output

Both programs will print:
```
Sum: <total>
Elapsed time: <ms> ms
```

Results will be compared manually (or via a short shell script) to illustrate the speedup from threading.

---

## Thread Safety Notes

- Each thread writes to a distinct row range of `resultMatrix` — no write conflicts
- Each thread has its own `sum` local variable — no shared mutable state
- Final sum accumulation happens after all threads `join()` — safe
