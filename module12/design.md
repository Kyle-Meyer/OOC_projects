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

```
int main() {
    // 1. Ask for number of players (2-7).
    // 2. For each player: ask name and "human" or "computer".
    // 3. Construct Player objects (starting chips e.g. 500).
    // 4. Ask for ante amount.
    // 5. Construct Game and call run().
}
```

---

## Game Flow Summary

```
main()
 └─ Game::run()
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

`CMakeLists.txt` at the module12 root:

```cmake
cmake_minimum_required(VERSION 3.15)
project(Poker CXX)
set(CMAKE_CXX_STANDARD 17)

file(GLOB SRC src/*.cpp)
add_executable(poker ${SRC})
target_include_directories(poker PRIVATE inc)
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
│   └── game.hpp
└── src/
    ├── pokerhand.cpp      (from module4)
    ├── deck.cpp
    ├── player.cpp
    ├── human_player.cpp
    ├── computer_player.cpp
    ├── game.cpp
    └── main.cpp
```
