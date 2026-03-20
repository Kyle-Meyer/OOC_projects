#pragma once

#include <vector>
#include "Floor.h"
#include "Elevator.h"

class Building {
public:
    std::vector<Floor>    floors;
    std::vector<Elevator> elevators;

    explicit Building(int floorTravelTime);

    Floor& getFloor(int n);               // 1-indexed
    void   spawnPassenger(Passenger* p);
};
