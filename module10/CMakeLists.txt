cmake_minimum_required(VERSION 3.15)
project(ElevatorSim)
set(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED COMPONENTS log log_setup)

add_executable(elevator_sim
    src/main.cpp
    src/Passenger.cpp
    src/Floor.cpp
    src/Elevator.cpp
    src/Building.cpp
    src/Simulation.cpp
)

target_link_libraries(elevator_sim
    Boost::log
    Boost::log_setup
)

target_compile_definitions(elevator_sim PRIVATE BOOST_LOG_DYN_LINK)
