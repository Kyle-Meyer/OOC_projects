#include "Floor.h"

void Floor::enqueue(Passenger* p) {
    waitingPassengers.push(p);
}

std::vector<Passenger*> Floor::dequeue(int count) {
    std::vector<Passenger*> result;
    while (count > 0 && !waitingPassengers.empty()) {
        result.push_back(waitingPassengers.front());
        waitingPassengers.pop();
        --count;
    }
    return result;
}

bool Floor::hasWaiting() const {
    return !waitingPassengers.empty();
}

int Floor::waitingCount() const {
    return static_cast<int>(waitingPassengers.size());
}
