# Module 12 — 5-Card Draw Poker: Design Document

## Overview

This program implements a 5-card draw poker game supporting 2–7 players (human or computer). Hand evaluation and comparison logic is reused from module4. New classes handle the deck, players, betting, and game flow.

---

## Reused from Module 4

Copy or symlink these directly into `inc/` and `src/`:

| File | Notes |
|---|---|
| `inc/card.hpp` | `Card` struct with `rank` (2–14) and `suit` ('C','D','H','S') |
| `inc/handrank.hpp` | `HandRank` enum (HighCard → StraightFlush) |
| `inc/pokerhand.hpp` | `PokerHand` class with hand evaluation and comparison |
| `src/pokerhand.cpp` | Implementation of `PokerHand` |

`PokerHand` will need one addition: a constructor that accepts `std::vector<Card>` directly (in addition to the existing string constructor), since in-game hands are built from dealt cards, not strings.

---

## New Files

### `inc/deck.hpp` / `src/deck.cpp` — `Deck`

Manages a standard 52-card deck.

```
class Deck {
public:
    Deck();                        // build all 52 cards
    void shuffle();                // Fisher-Yates in-place shuffle
    Card deal();                   // remove and return top card
    void reset();                  // rebuild and re-shuffle

private:
    std::vector<Card> cards_;
};
```

**Fisher-Yates shuffle:** iterate from index `n-1` down to `1`, swap `cards_[i]` with `cards_[rand() % (i+1)]`. Use `<random>` (`std::mt19937` seeded from `std::random_device`) rather than `rand()` for better randomness.

---

### `inc/player.hpp` / `src/player.cpp` — `Player` (abstract base)

```
class Player {
public:
    Player(const std::string& name, int chips);
    virtual ~Player() = default;

    // Called each betting round. Returns the amount bet (0 = check/fold).
    // currentBet is the highest bet at the table so far.
    virtual int placeBet(int currentBet, int pot) = 0;

    // Called during the draw phase.
    // Returns indices (0-4) of cards to discard.
    virtual std::vector<int> chooseDiscards() = 0;

    // Called to show whether the player has folded.
    virtual bool hasFolded() const { return folded_; }

    void receiveCard(const Card& c);
    void discardCards(const std::vector<int>& indices);
    void clearHand();
    void fold();
    void addChips(int amount);
    bool deductChips(int amount);   // returns false if insufficient

    const std::string& getName() const;
    int getChips() const;
    const std::vector<Card>& getHand() const;
    PokerHand evaluateHand() const;  // builds PokerHand from current cards_

protected:
    std::string name_;
    int chips_;
    bool folded_;
    std::vector<Card> cards_;   // always 5 cards during play
};
```

---

### `inc/human_player.hpp` / `src/human_player.cpp` — `HumanPlayer`

Overrides both pure-virtual methods with `std::cin`/`std::cout` prompts.

```
class HumanPlayer : public Player {
public:
    HumanPlayer(const std::string& name, int chips);

    int placeBet(int currentBet, int pot) override;
    // Prompts: "fold / check / call / raise <amount>"
    // Validates input and loops on bad input.

    std::vector<int> chooseDiscards() override;
    // Shows hand with indices, prompts for space-separated discard indices.
    // Allows discarding 0–3 cards (4 only if holding an Ace).
};
```

---

### `inc/computer_player.hpp` / `src/computer_player.cpp` — `ComputerPlayer`

Implements a simple but rule-following strategy.

```
class ComputerPlayer : public Player {
public:
    ComputerPlayer(const std::string& name, int chips);

    int placeBet(int currentBet, int pot) override;
    // Strategy:
    //   FourOfAKind or better → raise by 20
    //   FullHouse / Flush / Straight → raise by 10
    //   ThreeOfAKind / TwoPair → call
    //   OnePair → call if currentBet <= chips_/4, else fold
    //   HighCard → fold if currentBet > 0, else check

    std::vector<int> chooseDiscards() override;
    // Strategy: keep the best-ranked subset.
    //   FourOfAKind / FullHouse → keep all (discard none)
    //   ThreeOfAKind → keep the three, discard two kickers
    //   TwoPair → keep both pairs, discard one kicker
    //   OnePair → keep the pair, discard three kickers
    //   HighCard / Flush / Straight → discard none (keep hand)
};
```

---

### `inc/game.hpp` / `src/game.cpp` — `Game`

Orchestrates the full game loop.

```
class Game {
public:
    Game(std::vector<std::unique_ptr<Player>> players, int ante);

    void run();           // main loop: play rounds until one player has chips

private:
    void playRound();

    void dealHands();                         // deal 5 cards to each active player
    void bettingRound(const std::string& label);  // one pass of betting
    void drawPhase();                         // each player discards and redraws
    void showdown();                          // reveal hands, award pot

    void printTable() const;                  // show pot and player chip counts

    std::vector<std::unique_ptr<Player>> players_;
    Deck deck_;
    int pot_;
    int ante_;
    int currentBet_;     // highest bet this round
};
```

**Betting round logic (one pass):**
1. Reset `currentBet_` to 0.
2. Loop through non-folded players:
   - Call `player->placeBet(currentBet_, pot_)`.
   - If the returned amount raises the bet, set `currentBet_` and continue looping until everyone has matched.
3. Collect chips into `pot_`.

---

### `src/main.cpp`

Accepts an optional `--gui` / `-g` flag. If present, delegates to `runGui()` and returns. Otherwise falls through to the original CLI setup and game loop.

```
int main(int argc, char* argv[]) {
    if --gui flag present → runGui(); return 0;

    // CLI mode:
    // 1. Ask for number of players (2-7).
    // 2. For each player: ask name and "human" or "computer".
    // 3. Construct Player objects.
    // 4. Ask for ante amount.
    // 5. Construct Game and call run().
}
```

---

## Extra Credit — Dear ImGui GUI (--gui flag)

Running `./poker --gui` (or `./poker -g`) opens a windowed GUI built with [Dear ImGui](https://github.com/ocornut/imgui) + GLFW + OpenGL 3.

### Architecture

The game logic is completely unchanged. The GUI layer adds three new components:

| Component | Role |
|---|---|
| `GuiState` | Shared data between the game thread and the render thread |
| `GuiPlayer` | `Player` subclass whose `placeBet` / `chooseDiscards` block on a condition variable until the UI provides input |
| `runGui()` | GLFW/ImGui render loop; also owns the game thread |

**Thread model:** `game.run()` executes in a `std::thread`. The main thread runs the ImGui render loop. `GuiPlayer` communicates with the render thread via `GuiState` (mutex + `std::condition_variable`).

**stdout capture:** A custom `LogBuf` (`std::streambuf` subclass) is installed on `std::cout` inside the game thread. It appends each line to `GuiState::log`, which the log panel reads under the mutex.

### `GuiState` (shared state)

```cpp
enum class GuiAction { None, Bet, Discard };

struct GuiState {
    std::mutex mtx;
    std::condition_variable cv;

    std::vector<std::string> log;       // captured stdout lines

    GuiAction         action;           // what the UI should show
    int               toCall, pot;      // bet context
    std::vector<Card> hand;             // current hand to display
    std::vector<bool> discardSel;       // toggled by the user (size 5)

    bool actionReady;                   // set by render thread to unblock GuiPlayer
    int  betResult;                     // -1 = fold, >=0 = chips put in
    bool gameOver;
};
```

### `GuiPlayer` flow

`placeBet`:
1. Lock → set `action = Bet`, populate `toCall / pot / hand`, clear `actionReady` → unlock.
2. `cv.wait` until `actionReady`.
3. Read `betResult` (-1 = fold) and return it.

`chooseDiscards`:
1. Lock → set `action = Discard`, populate `hand`, reset `discardSel` to all-false → unlock.
2. `cv.wait` until `actionReady`.
3. Collect indices where `discardSel[i]` is true and return them.

### UI layout (960×680 window)

```
┌─────────────────────────┬──────────────────────┐
│                         │                      │
│      Game Log           │   Action Panel       │
│  (captured std::cout)   │  (Bet or Discard or  │
│                         │   Waiting/Game Over) │
│      55% width          │      45% width       │
└─────────────────────────┴──────────────────────┘
```

**Setup screen** (shown before the game starts): sliders/inputs for player count, starting chips, ante, and per-player name + human/CPU toggle.

**Bet panel** (when it's the human's turn to bet): shows hand as card buttons, then Fold / Check / Call / Raise controls.

**Discard panel**: shows 5 card buttons that toggle orange when selected for discard; a "Confirm Discards" button submits.

### New files

| File | Purpose |
|---|---|
| `inc/gui_state.hpp` | `GuiAction` enum + `GuiState` struct |
| `inc/gui_player.hpp` | `GuiPlayer` class declaration |
| `inc/gui_app.hpp` | `void runGui()` declaration |
| `src/gui_player.cpp` | `GuiPlayer` implementation |
| `src/gui_app.cpp` | `LogBuf`, card rendering helpers, `runGui()` |

---

## Game Flow Summary

```
main()
 ├─ --gui → runGui()
 │    ├─ Setup screen (ImGui)
 │    ├─ Launch std::thread → Game::run()
 │    │    └─ (GuiPlayer::placeBet / chooseDiscards block on GuiState::cv)
 │    └─ ImGui render loop (signals cv when user acts)
 │
 └─ (CLI) Game::run()
     └─ loop while >1 player has chips:
         Game::playRound()
          ├─ collect antes → pot_
          ├─ Deck::reset() + Deck::shuffle()
          ├─ Game::dealHands()          (5 cards each)
          ├─ Game::bettingRound("Pre-draw")
          ├─ Game::drawPhase()          (discard + redeal)
          ├─ Game::bettingRound("Post-draw")
          └─ Game::showdown()           (PokerHand::compareTo, award pot)
```

---

## Hand Comparison for Showdown

Use the existing `PokerHand::compareTo()` from module4. Iterate over all non-folded players, build a `PokerHand` from each player's `cards_`, and find the maximum. If two hands compare equal, split the pot.

---

## Build System

`CMakeLists.txt` uses `FetchContent` to download Dear ImGui at configure time, then defines an `imgui` static library target with the GLFW + OpenGL3 backends. GLFW and OpenGL must be installed on the host (both are available on this system).

```cmake
cmake_minimum_required(VERSION 3.15)
project(Poker CXX)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(imgui GIT_REPOSITORY https://github.com/ocornut/imgui.git GIT_TAG v1.91.6)
FetchContent_MakeAvailable(imgui)

add_library(imgui STATIC <imgui sources + GLFW/OpenGL3 backends>)
target_link_libraries(imgui PUBLIC OpenGL::GL glfw)

file(GLOB SRC src/*.cpp)
add_executable(poker ${SRC})
target_include_directories(poker PRIVATE inc)
target_link_libraries(poker PRIVATE imgui)
```

Build and run:
```bash
cmake .. && make
./poker         # CLI mode
./poker --gui   # GUI mode
```

---

## File Tree

```
module12/
├── CMakeLists.txt
├── inc/
│   ├── card.hpp           (from module4)
│   ├── handrank.hpp       (from module4)
│   ├── pokerhand.hpp      (from module4, +vector<Card> ctor)
│   ├── deck.hpp
│   ├── player.hpp
│   ├── human_player.hpp
│   ├── computer_player.hpp
│   ├── game.hpp
│   ├── gui_state.hpp      (extra credit GUI)
│   ├── gui_player.hpp     (extra credit GUI)
│   └── gui_app.hpp        (extra credit GUI)
└── src/
    ├── pokerhand.cpp      (from module4)
    ├── deck.cpp
    ├── player.cpp
    ├── human_player.cpp
    ├── computer_player.cpp
    ├── game.cpp
    ├── main.cpp
    ├── gui_player.cpp     (extra credit GUI)
    └── gui_app.cpp        (extra credit GUI)
```
