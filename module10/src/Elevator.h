#pragma once

#include <vector>
#include "Floor.h"

enum class ElevatorState { STOPPED, STOPPING, MOVING_UP, MOVING_DOWN };

class Elevator {
public:
    int                     id;
    int                     currentFloor;
    ElevatorState           state;
    std::vector<Passenger*> onBoard;
    int                     floorTimer;
    int                     stopTimer;
    int                     floorTravelTime;
    int                     targetFloor;

    Elevator(int id, int floorTravelTime);

    void update(int currentTime, std::vector<Floor>& floors);
    bool shouldStopAt(int floor, const std::vector<Floor>& floors) const;
    void boardPassengers(Floor& floor, int currentTime);
    void dischargePassengers(int currentTime);
    bool hasWork(const std::vector<Floor>& floors) const;

private:
    int determineTargetFloor(const std::vector<Floor>& floors) const;
};
