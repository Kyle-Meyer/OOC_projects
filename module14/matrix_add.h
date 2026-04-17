#pragma once

#define NUM_ROWS 1000
#define NUM_COLS 1000
#define NUM_THREADS 4

void matrixAdd(double leftMatrix[NUM_ROWS][NUM_COLS],
               double rightMatrix[NUM_ROWS][NUM_COLS],
               double resultMatrix[NUM_ROWS][NUM_COLS],
               int startRow,
               int endRow,
               double &sum)
{
    sum = 0;
    for (int row = startRow; row <= endRow; row++)
    {
        for (int col = 0; col < NUM_COLS; col++)
        {
            int value = leftMatrix[row][col] + rightMatrix[row][col];
            resultMatrix[row][col] = value;
            sum += value;
        }
    }
}
