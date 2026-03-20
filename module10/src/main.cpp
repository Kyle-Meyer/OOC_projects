#include <iostream>
#include <iomanip>
#include "Logger.h"
#include "Simulation.h"

int main() {
    Logger::init("simulation.log");

    // ── Run 1: 10 s/floor ──────────────────────────────────────────────────
    std::cout << "=== Simulation Run 1: 10 seconds/floor ===\n";
    LOG_INFO  << "=== Simulation Run 1: 10 seconds/floor ===";

    Simulation sim(10);
    sim.loadCSV("Elevator.csv");
    sim.run();
    sim.reportStatistics();

    // Save run-1 averages for comparison
    double totalWait1 = 0.0, totalTravel1 = 0.0;
    for (const Passenger& p : sim.passengers) {
        totalWait1   += p.waitTime();
        totalTravel1 += p.travelTime();
    }
    double n       = static_cast<double>(sim.passengers.size());
    double avgWait1   = totalWait1   / n;
    double avgTravel1 = totalTravel1 / n;

    // ── Run 2: 5 s/floor ───────────────────────────────────────────────────
    std::cout << "\n=== Simulation Run 2: 5 seconds/floor ===\n";
    LOG_INFO  << "=== Simulation Run 2: 5 seconds/floor ===";

    sim.reset(5);
    sim.run();

    double totalWait2 = 0.0, totalTravel2 = 0.0;
    for (const Passenger& p : sim.passengers) {
        totalWait2   += p.waitTime();
        totalTravel2 += p.travelTime();
    }
    double avgWait2   = totalWait2   / n;
    double avgTravel2 = totalTravel2 / n;

    double waitReduction   = (avgWait1   - avgWait2)   / avgWait1   * 100.0;
    double travelReduction = (avgTravel1 - avgTravel2) / avgTravel1 * 100.0;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Average wait time  : " << avgWait2
              << " s  (" << waitReduction   << "% reduction)\n";
    std::cout << "Average travel time: " << avgTravel2
              << " s  (" << travelReduction << "% reduction)\n";

    LOG_INFO << "Run 2 - Average wait time: "    << avgWait2
             << " s (" << waitReduction << "% reduction)"
             << ", Average travel time: "         << avgTravel2
             << " s (" << travelReduction << "% reduction)";

    return 0;
}
