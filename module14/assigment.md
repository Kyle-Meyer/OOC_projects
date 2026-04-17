# Assignment Description
Using the code provided for Matrix Addition, execute a unthreaded and a threaded application to determine the difference in execution times.

# Participation guidelines
Use the following algorithm

```c++
// Add two matrices returning the sum of all elements of the result
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
} // end matrixAdd
```

