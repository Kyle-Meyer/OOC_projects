#pragma once

enum class PassengerState { WAITING, RIDING, ARRIVED };

class Passenger {
public:
    int           id;
    int           startTime;
    int           startFloor;
    int           endFloor;
    int           boardTime;
    int           exitTime;
    PassengerState state;

    Passenger(int id, int startTime, int startFloor, int endFloor);

    int waitTime()   const { return boardTime - startTime; }
    int travelTime() const { return exitTime  - boardTime; }

    void board(int time);
    void arrive(int time);
    void reset();
};
