#include <iostream>
#include <chrono>
#include <cstdlib>
#include <thread>
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

    const int rowsPerThread = NUM_ROWS / NUM_THREADS;
    std::thread threads[NUM_THREADS];
    double sums[NUM_THREADS] = {};

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < NUM_THREADS; i++)
    {
        int startRow = i * rowsPerThread;
        int endRow   = (i == NUM_THREADS - 1) ? NUM_ROWS - 1 : startRow + rowsPerThread - 1;
        threads[i] = std::thread(matrixAdd, left, right, result, startRow, endRow, std::ref(sums[i]));
    }

    for (int i = 0; i < NUM_THREADS; i++)
        threads[i].join();

    auto end = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    double total = 0;
    for (int i = 0; i < NUM_THREADS; i++)
        total += sums[i];

    std::cout << "Sum:          " << total << "\n";
    std::cout << "Elapsed time: " << ms << " ms\n";

    return 0;
}
