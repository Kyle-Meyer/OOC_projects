#include "Elevator.h"
#include "Logger.h"
#include <algorithm>
#include <climits>
#include <cstdlib>

Elevator::Elevator(int id, int floorTravelTime)
    : id(id), currentFloor(1), state(ElevatorState::STOPPED),
      floorTimer(0), stopTimer(0), floorTravelTime(floorTravelTime), targetFloor(1)
{}

void Elevator::update(int currentTime, std::vector<Floor>& floors) {
    switch (state) {

    case ElevatorState::STOPPED: {
        dischargePassengers(currentTime);
        boardPassengers(floors[currentFloor - 1], currentTime);

        if (hasWork(floors)) {
            int target = determineTargetFloor(floors);
            if (target > currentFloor) {
                targetFloor = target;
                state = ElevatorState::MOVING_UP;
                LOG_INFO << "Elevator " << id << " moving UP   from floor "
                         << currentFloor << " toward floor " << target;
            } else if (target < currentFloor) {
                targetFloor = target;
                state = ElevatorState::MOVING_DOWN;
                LOG_INFO << "Elevator " << id << " moving DOWN from floor "
                         << currentFloor << " toward floor " << target;
            }
            // target == currentFloor cannot happen in normal operation;
            // if it somehow does, stay STOPPED and re-evaluate next tick.
            floorTimer = floorTravelTime;
        }
        break;
    }

    case ElevatorState::MOVING_UP: {
        --floorTimer;
        if (floorTimer == 0) {
            ++currentFloor;
            floorTimer = floorTravelTime;
            LOG_DEBUG << "Elevator " << id << " passed floor " << currentFloor;
            if (currentFloor >= static_cast<int>(floors.size()) ||
                currentFloor == targetFloor ||
                shouldStopAt(currentFloor, floors)) {
                state     = ElevatorState::STOPPING;
                stopTimer = 2;
                LOG_INFO << "Elevator " << id << " stopping at floor " << currentFloor;
            }
        }
        break;
    }

    case ElevatorState::MOVING_DOWN: {
        --floorTimer;
        if (floorTimer == 0) {
            --currentFloor;
            floorTimer = floorTravelTime;
            LOG_DEBUG << "Elevator " << id << " passed floor " << currentFloor;
            if (currentFloor <= 1 ||
                currentFloor == targetFloor ||
                shouldStopAt(currentFloor, floors)) {
                state     = ElevatorState::STOPPING;
                stopTimer = 2;
                LOG_INFO << "Elevator " << id << " stopping at floor " << currentFloor;
            }
        }
        break;
    }

    case ElevatorState::STOPPING: {
        --stopTimer;
        if (stopTimer == 0) {
            state = ElevatorState::STOPPED;
            LOG_DEBUG << "Elevator " << id << " is now STOPPED at floor " << currentFloor;
        }
        break;
    }

    } // switch
}

bool Elevator::shouldStopAt(int floor, const std::vector<Floor>& floors) const {
    // Stop to discharge passengers destined for this floor
    for (const Passenger* p : onBoard) {
        if (p->endFloor == floor) return true;
    }
    // Stop to board passengers waiting at this floor
    if (floors[floor - 1].hasWaiting()) return true;
    return false;
}

void Elevator::boardPassengers(Floor& floor, int currentTime) {
    int capacity = 8 - static_cast<int>(onBoard.size());
    if (capacity <= 0) return;
    auto boarded = floor.dequeue(capacity);
    for (Passenger* p : boarded) {
        p->board(currentTime);
        onBoard.push_back(p);
        LOG_INFO << "Passenger " << p->id << " boarded elevator " << id
                 << " at floor " << currentFloor << " at t=" << currentTime;
    }
}

void Elevator::dischargePassengers(int currentTime) {
    auto it = onBoard.begin();
    while (it != onBoard.end()) {
        Passenger* p = *it;
        if (p->endFloor == currentFloor) {
            p->arrive(currentTime);
            LOG_INFO << "Passenger " << p->id << " exited elevator " << id
                     << " at floor " << currentFloor << " at t=" << currentTime
                     << " (wait=" << p->waitTime() << "s, travel=" << p->travelTime() << "s)";
            it = onBoard.erase(it);
        } else {
            ++it;
        }
    }
}

bool Elevator::hasWork(const std::vector<Floor>& floors) const {
    if (!onBoard.empty()) return true;
    for (const Floor& f : floors) {
        if (f.hasWaiting()) return true;
    }
    return false;
}

int Elevator::determineTargetFloor(const std::vector<Floor>& floors) const {
    // 1. Move toward first on-board passenger's destination
    if (!onBoard.empty()) {
        return onBoard[0]->endFloor;
    }
    // 2. Move toward nearest floor with waiting passengers
    int nearest = currentFloor;
    int minDist = INT_MAX;
    for (int i = 0; i < static_cast<int>(floors.size()); ++i) {
        if (floors[i].hasWaiting()) {
            int dist = std::abs((i + 1) - currentFloor);
            if (dist < minDist) {
                minDist = dist;
                nearest = i + 1;
            }
        }
    }
    return nearest;
}
