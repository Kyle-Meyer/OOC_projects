Elevators
There are 4 elevators available to take passengers up and down the floors of a 100 floor
building. Each Elevator can be STOPPED, STOPPING, MOVING_UP, or MOVING_DOWN.
It takes an elevator 2 seconds of STOPPING to be STOPPED. It currently takes each
elevator 10 seconds of moving to move between each floor which does not include the stopping time.
There is a maximum of 8 passengers allowed in a single elevator.
Building passengers are complaining about the length of time they have to wait for elevators
and the time it takes for them to get to their destination.
Implement an Object Oriented simulation, based on the set of Start Time(s) in seconds, Start
Floor, and End Floor from Elevator.csv, and determine the average wait time and the average
travel time it takes for passengers. A passenger arrives at the Start Floor at the Start Time,
waits for the elevator to arrive and stays on the elevator until arriving at the End Floor. When
a elevator arrives at a floor, it picks up all of the passengers waiting at the floor, up to the
maximum capacity of 8, and then starts moving to the End Floors.
The Objects should be as a minimum the Elevators, the Passengers, and Floors, each with
attributes and behaviors as well as relations to other objects. Passengers should be queued
waiting, if necessary, for elevators. The simulation performs one loop each [simulated]
second. Every loop each object decides what to do. If STOPPED and current time is equal
to or greater than the next Start time, it either begins MOVING_UP or MOVING _DOWN
depending on the direction needed to pick up the passenger. If MOVING_UP or
MOVING_DOWN and there are one or more passengers waiting at a floor along the way or
current passengers with End Floor being the current floor, it begins STOPPING. When
STOPPED at a floor if there are any passengers in the elevator with that End Floor as a
destination it discharges the passengers. Then, if there are any passengers with that floor as
their Start Floor it picks up passengers up to the capacity of the elevator. Only one elevator
can pick up a passenger. There is no central controller of elevators as ach elevator is making
its own decisions as to which passengers to pick up and discharge, therefore there are no
conflicts. As this is merely a simulation, the logic can be extremely simple and maybe
inefficient, but all passengers should be picked up and arrive at destinations. Wherever
possible, data structures should be STL.
A contractor has proposed, for $50,000, to speed up the elevators from 10 seconds to 5
seconds between each floor. Rerun the simulation with a time between each floor of 5
seconds, to determine the % reduction of average wait time and the % reduction of average
travel time for passengers that could be achieved.
Submit Source Code and program output as a single .pdf to Blackboard
Grading:
50% for correct and complete
30% design
20% style
Extra Credit worth up to 10% - Log the output from the simulation using a logger such as log4cxx,
spdlog, glog or other logger
Maximum possible grade is 100
