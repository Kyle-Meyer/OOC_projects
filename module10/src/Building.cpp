#include "Building.h"

Building::Building(int floorTravelTime) {
    floors.reserve(100);
    for (int i = 1; i <= 100; ++i) {
        floors.emplace_back(i);
    }
    elevators.reserve(4);
    for (int i = 0; i < 4; ++i) {
        elevators.emplace_back(i, floorTravelTime);
    }
}

Floor& Building::getFloor(int n) {
    return floors[n - 1];
}

void Building::spawnPassenger(Passenger* p) {
    floors[p->startFloor - 1].enqueue(p);
}
