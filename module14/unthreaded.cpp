#include <iostream>
#include <chrono>
#include <cstdlib>
#include "matrix_add.h"

static double left[NUM_ROWS][NUM_COLS];
static double right[NUM_ROWS][NUM_COLS];
static double result[NUM_ROWS][NUM_COLS];

int main()
{
    for (int r = 0; r < NUM_ROWS; r++)
        for (int c = 0; c < NUM_COLS; c++)
        {
            left[r][c]  = rand() % 100;
            right[r][c] = rand() % 100;
        }

    double sum = 0;
    auto start = std::chrono::high_resolution_clock::now();

    matrixAdd(left, right, result, 0, NUM_ROWS - 1, sum);

    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    std::cout << "Sum:          " << sum << "\n";
    std::cout << "Elapsed time: " << ms << " ms\n";

    return 0;
}
