#include "Passenger.h"

Passenger::Passenger(int id, int startTime, int startFloor, int endFloor)
    : id(id), startTime(startTime), startFloor(startFloor), endFloor(endFloor),
      boardTime(-1), exitTime(-1), state(PassengerState::WAITING)
{}

void Passenger::board(int time) {
    boardTime = time;
    state     = PassengerState::RIDING;
}

void Passenger::arrive(int time) {
    exitTime = time;
    state    = PassengerState::ARRIVED;
}

void Passenger::reset() {
    boardTime = -1;
    exitTime  = -1;
    state     = PassengerState::WAITING;
}
