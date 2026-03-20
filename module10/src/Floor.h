#pragma once

#include <queue>
#include <vector>
#include "Passenger.h"

class Floor {
public:
    int                       floorNumber;
    std::queue<Passenger*>    waitingPassengers;

    explicit Floor(int n) : floorNumber(n) {}

    void                    enqueue(Passenger* p);
    std::vector<Passenger*> dequeue(int count);
    bool                    hasWaiting()    const;
    int                     waitingCount()  const;
};
