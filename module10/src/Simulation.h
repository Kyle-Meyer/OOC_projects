#pragma once

#include <string>
#include <vector>
#include "Building.h"
#include "Passenger.h"

class Simulation {
public:
    Building              building;
    std::vector<Passenger> passengers;
    int                   currentTime;
    int                   floorTravelTime;

    explicit Simulation(int floorTravelTime);

    void loadCSV(const std::string& filename);
    void run();
    void reportStatistics() const;
    void reset(int newFloorTime);

private:
    bool allPassengersArrived() const;
};
