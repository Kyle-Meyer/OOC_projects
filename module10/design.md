# Design Document: Elevator Simulation (Module 10)

## Overview

An object-oriented, discrete-time simulation of a 4-elevator, 100-floor building. The simulation
reads passenger requests from `Elevator.csv`, runs second-by-second, and reports average wait time
and average travel time. A second run uses 5 s/floor to evaluate the contractor's proposal.
All simulation events are logged with Boost.log.

---

## Requirements Summary

| Parameter | Value |
|---|---|
| Floors | 100 |
| Elevators | 4 |
| Elevator states | STOPPED, STOPPING, MOVING_UP, MOVING_DOWN |
| Time per floor | 10 s (fast mode: 5 s) |
| Stopping duration | 2 s |
| Max capacity | 8 passengers |
| Input | `Elevator.csv` (StartTime, StartFloor, EndFloor) |
| Output | Average wait time, average travel time; % reduction for fast mode |
| Extra credit | Boost.log logging |

---

## File / Directory Structure

```
module10/
├── CMakeLists.txt
├── Elevator.csv
├── design.md
└── src/
    ├── main.cpp
    ├── Logger.h          # Boost.log wrapper (header-only)
    ├── Passenger.h
    ├── Passenger.cpp
    ├── Floor.h
    ├── Floor.cpp
    ├── Elevator.h
    ├── Elevator.cpp
    ├── Building.h
    ├── Building.cpp
    ├── Simulation.h
    └── Simulation.cpp
```

---

## Class Design

### `ElevatorState` (enum class)
```cpp
enum class ElevatorState { STOPPED, STOPPING, MOVING_UP, MOVING_DOWN };
```

### `PassengerState` (enum class)
```cpp
enum class PassengerState { WAITING, RIDING, ARRIVED };
```

---

### `Passenger`

Represents a single building occupant parsed from the CSV.

**Attributes**

| Member | Type | Description |
|---|---|---|
| `id` | `int` | Unique identifier |
| `startTime` | `int` | Simulation second they arrive at start floor |
| `startFloor` | `int` | Floor they are waiting on |
| `endFloor` | `int` | Floor they want to reach |
| `boardTime` | `int` | Simulation second they board an elevator (-1 until set) |
| `exitTime` | `int` | Simulation second they exit an elevator (-1 until set) |
| `state` | `PassengerState` | Current state |

**Computed properties**

- `waitTime()` → `boardTime - startTime`
- `travelTime()` → `exitTime - boardTime`

**Behaviors**

- `board(int time)` — sets `boardTime`, transitions state to RIDING
- `arrive(int time)` — sets `exitTime`, transitions state to ARRIVED

---

### `Floor`

Represents one of 100 floors. Holds the queue of waiting passengers.

**Attributes**

| Member | Type | Description |
|---|---|---|
| `floorNumber` | `int` | 1–100 |
| `waitingPassengers` | `std::queue<Passenger*>` | Passengers waiting to board |

**Behaviors**

- `enqueue(Passenger*)` — add a passenger to the waiting queue
- `dequeue(int count)` → `std::vector<Passenger*>` — remove up to `count` passengers and return them
- `hasWaiting() const` → `bool`
- `waitingCount() const` → `int`

---

### `Elevator`

Core simulation actor. Makes all decisions autonomously each tick.

**Attributes**

| Member | Type | Description |
|---|---|---|
| `id` | `int` | Elevator identifier (0–3) |
| `currentFloor` | `int` | Current floor position (1–100) |
| `state` | `ElevatorState` | Current state |
| `onBoard` | `std::vector<Passenger*>` | Passengers currently in the elevator |
| `floorTimer` | `int` | Counts down seconds until next floor arrival |
| `stopTimer` | `int` | Counts down seconds remaining in STOPPING |
| `floorTravelTime` | `int` | Seconds per floor (10 or 5) |

**Behaviors**

- `update(int currentTime, std::vector<Floor>& floors)` — main per-tick logic (see state machine)
- `shouldStopAt(int floor, const std::vector<Floor>& floors) const` → `bool`
  - Returns true if any on-board passenger has `endFloor == floor`, OR the floor has waiting passengers along the direction of travel
- `boardPassengers(Floor& floor, int currentTime)` — dequeue up to `(8 - onBoard.size())` passengers, call `passenger->board(currentTime)`
- `dischargePassengers(int currentTime)` — remove and mark arrived all on-board passengers whose `endFloor == currentFloor`
- `hasWork(const std::vector<Floor>& floors) const` → `bool` — true if on-board passengers remain OR any floor has waiting passengers

#### State Machine

```
STOPPED
  ├─ discharge passengers with endFloor == currentFloor
  ├─ board passengers from currentFloor queue
  └─ if hasWork():
       └─ determine direction (toward nearest unserved passenger)
            → MOVING_UP or MOVING_DOWN  (floorTimer = floorTravelTime)

MOVING_UP / MOVING_DOWN
  ├─ decrement floorTimer each tick
  └─ when floorTimer == 0:
       ├─ advance currentFloor (±1)
       ├─ reset floorTimer = floorTravelTime
       └─ if shouldStopAt(currentFloor):
            → STOPPING  (stopTimer = 2)

STOPPING
  ├─ decrement stopTimer each tick
  └─ when stopTimer == 0:
       → STOPPED  (triggers discharge + board next tick)
```

**Direction decision (STOPPED with work):**
1. If on-board passengers exist, move toward the first on-board passenger's `endFloor`.
2. Else, find the floor with the earliest-arrived waiting passenger and move toward it.

---

### `Building`

Aggregates elevators and floors; provides access helpers.

**Attributes**

| Member | Type | Description |
|---|---|---|
| `floors` | `std::vector<Floor>` | 100 floors (index 0 = floor 1) |
| `elevators` | `std::vector<Elevator>` | 4 elevators |

**Behaviors**

- `getFloor(int n)` → `Floor&` (1-indexed)
- `spawnPassenger(Passenger* p)` — enqueue passenger onto their start floor
- Constructor initializes all floors and elevators

---

### `Simulation`

Orchestrates the simulation loop.

**Attributes**

| Member | Type | Description |
|---|---|---|
| `building` | `Building` | The building instance |
| `passengers` | `std::vector<Passenger>` | All passengers loaded from CSV |
| `currentTime` | `int` | Current simulation second |
| `floorTravelTime` | `int` | 10 or 5 |

**Behaviors**

- `loadCSV(const std::string& filename)` — parse `Elevator.csv`, populate `passengers`
- `run()` — execute simulation loop until all passengers ARRIVED
- `reportStatistics() const` — compute and print average wait time, average travel time
- `reset(int newFloorTime)` — reinitialize building and passengers for a second run

#### Simulation Loop (pseudocode)

```
while not allPassengersArrived():
    for each passenger p where p.startTime == currentTime and p.state == WAITING:
        building.spawnPassenger(&p)
        LOG_INFO: "Passenger {id} arrives at floor {f} at t={t}"

    for each elevator e in building.elevators:
        e.update(currentTime, building.floors)

    LOG_DEBUG: snapshot of each elevator state and floor

    currentTime++
```

---

### `Logger` (header-only Boost.log wrapper)

Wraps Boost.log initialization and provides convenience macros.

**Severity levels used:** `trace`, `debug`, `info`, `warning`, `error`

**Setup:** `Logger::init(filename)` — initializes a file sink (`simulation.log`) and a console sink (info+ only).

**Usage macros:**
```cpp
LOG_INFO  << "Elevator " << id << " moving up from floor " << currentFloor;
LOG_DEBUG << "Elevator " << id << " state: STOPPING, timer=" << stopTimer;
```

**Key events to log:**
- Passenger spawned at floor
- Passenger boards elevator
- Passenger exits elevator (with wait time and travel time)
- Elevator state transitions
- Each elevator's floor arrival
- Per-tick summary (optional, DEBUG level)
- Final statistics

---

## CSV Format

```
StartTime,StartFloor,EndFloor
0,1,10
5,3,50
...
```

Parsed with standard `std::ifstream` + `std::getline` / `std::stoi`.

---

## Build System (CMakeLists.txt)

```cmake
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
```

---

## Output

### Console / Log (10 s/floor run)

```
=== Simulation Run 1: 10 seconds/floor ===
[INFO] All 200 passengers arrived at t=4823
Average wait time  : 142.3 s
Average travel time: 310.7 s
```

### Console / Log (5 s/floor run)

```
=== Simulation Run 2: 5 seconds/floor ===
[INFO] All 200 passengers arrived at t=2614
Average wait time  : 87.1 s  (38.8% reduction)
Average travel time: 188.2 s (39.4% reduction)
```

---

## Data Structure Choices

| Structure | STL Type | Rationale |
|---|---|---|
| Waiting passengers per floor | `std::queue<Passenger*>` | FIFO boarding order |
| On-board passengers | `std::vector<Passenger*>` | Random removal on discharge |
| All passengers | `std::vector<Passenger>` | Contiguous, indexed access |
| Floors | `std::vector<Floor>` | Random access by floor number |
| Elevators | `std::vector<Elevator>` | Fixed 4-element collection |

---

## Key Design Decisions

1. **No central controller** — each `Elevator::update()` reads `Building::floors` directly, consistent with the assignment's autonomous-elevator requirement.
2. **Conflict avoidance** — when an elevator boards passengers, they are immediately dequeued from the floor. Since only one elevator calls `Floor::dequeue()` per tick per floor (elevators on different floors update independently), no two elevators can claim the same passenger.
3. **Simple direction heuristic** — move toward nearest unserved destination. This is intentionally naive per the assignment ("logic can be extremely simple and maybe inefficient").
4. **Two-pass main()** — `main()` calls `sim.run()` + `sim.reportStatistics()` twice, reconstructing the simulation with `floorTravelTime = 5` for the contractor comparison.
5. **Boost.log** — initialized once in `main()` before the first run; both runs share the same log file.
