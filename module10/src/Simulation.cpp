#include "Simulation.h"
#include "Logger.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>

Simulation::Simulation(int floorTravelTime)
    : building(floorTravelTime), currentTime(0), floorTravelTime(floorTravelTime)
{}

void Simulation::loadCSV(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        LOG_ERROR << "Failed to open " << filename;
        return;
    }

    std::string line;
    std::getline(file, line); // skip header

    int id = 0;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string token;
        int startTime, startFloor, endFloor;
        std::getline(ss, token, ','); startTime  = std::stoi(token);
        std::getline(ss, token, ','); startFloor = std::stoi(token);
        std::getline(ss, token, ','); endFloor   = std::stoi(token);
        passengers.emplace_back(id++, startTime, startFloor, endFloor);
    }

    LOG_INFO << "Loaded " << passengers.size() << " passengers from " << filename;
}

void Simulation::run() {
    currentTime = 0;

    while (!allPassengersArrived()) {
        // Spawn passengers whose start time has arrived
        for (Passenger& p : passengers) {
            if (p.startTime == currentTime && p.state == PassengerState::WAITING) {
                building.spawnPassenger(&p);
                LOG_INFO << "Passenger " << p.id << " arrives at floor "
                         << p.startFloor << " at t=" << currentTime;
            }
        }

        // Each elevator makes its own decision
        for (Elevator& e : building.elevators) {
            e.update(currentTime, building.floors);
        }

        LOG_DEBUG << "t=" << currentTime
                  << " E0@" << building.elevators[0].currentFloor
                  << " E1@" << building.elevators[1].currentFloor
                  << " E2@" << building.elevators[2].currentFloor
                  << " E3@" << building.elevators[3].currentFloor;

        ++currentTime;
    }

    LOG_INFO << "All " << passengers.size()
             << " passengers arrived at t=" << currentTime;
    std::cout << "[INFO] All " << passengers.size()
              << " passengers arrived at t=" << currentTime << "\n";
}

void Simulation::reportStatistics() const {
    double totalWait = 0.0, totalTravel = 0.0;
    for (const Passenger& p : passengers) {
        totalWait   += p.waitTime();
        totalTravel += p.travelTime();
    }
    double n        = static_cast<double>(passengers.size());
    double avgWait  = totalWait   / n;
    double avgTravel = totalTravel / n;

    std::cout << std::fixed << std::setprecision(1);
    std::cout << "Average wait time  : " << avgWait   << " s\n";
    std::cout << "Average travel time: " << avgTravel << " s\n";

    LOG_INFO << "Average wait time: "   << avgWait
             << " s, Average travel time: " << avgTravel << " s";
}

void Simulation::reset(int newFloorTime) {
    floorTravelTime = newFloorTime;
    building        = Building(newFloorTime);
    for (Passenger& p : passengers) {
        p.reset();
    }
    currentTime = 0;
}

bool Simulation::allPassengersArrived() const {
    for (const Passenger& p : passengers) {
        if (p.state != PassengerState::ARRIVED) return false;
    }
    return true;
}
