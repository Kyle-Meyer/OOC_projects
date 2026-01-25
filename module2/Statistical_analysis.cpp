#include <iostream>
#include <cmath>
#include <stdexcept>

class Statistic
{
public:
    // Constructor: initialize private data
    Statistic();

    // Add an item to the statistics
    void add(double x);

    // Get average
    double average() const;

    // Get Standard deviation
    double STD() const;

private:
    // Private member data
    int count;
    double sum;
    double sumOfSquares;
};

// Constructor implementation
Statistic::Statistic()
    : count(0), sum(0.0), sumOfSquares(0.0)
{
}

// Add a value to the statistics
void Statistic::add(double x)
{
    count++;
    sum += x;
    sumOfSquares += (x * x);
}

// Calculate and return the average
double Statistic::average() const
{
    if (count == 0) {
        throw std::runtime_error("Cannot calculate average of empty dataset");
    }
    return sum / count;
}

// Calculate and return the standard deviation
double Statistic::STD() const
{
    if (count == 0) {
        throw std::runtime_error("Cannot calculate STD of empty dataset");
    }

    // Using computational formula: STD = sqrt((sum of x²)/n - (sum of x/n)²)
    double avg = average();
    double variance = (sumOfSquares / count) - (avg * avg);

    // Handle potential floating-point errors that might make variance slightly negative
    if (variance < 0.0) {
        variance = 0.0;
    }

    return std::sqrt(variance);
}

// Test program
int main()
{
    Statistic stats;

    // Example sequence of numbers - replace with your discussion numbers
    stats.add(10.5);
    stats.add(12.3);
    stats.add(9.8);
    stats.add(11.2);
    stats.add(10.9);
    stats.add(13.1);
    stats.add(8.7);

    try {
        std::cout << "Average: " << stats.average() << std::endl;
        std::cout << "Standard Deviation: " << stats.STD() << std::endl;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}