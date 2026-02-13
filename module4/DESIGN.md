# Poker Hand Evaluator - Design Document

## Overview

A C++ class (`PokerHand`) that represents a five-card poker hand, determines its rank, and supports comparison between any two hands to decide which ranks higher.

## Input Format

The constructor accepts a single `std::string` of five space-separated two-character cards:

- **Character 1 (Rank):** `2 3 4 5 6 7 8 9 T J Q K A`
- **Character 2 (Suit):** `C D H S` (Clubs, Diamonds, Hearts, Spades)

Example: `"9H 7S TH JS 8D"` — no assumed ordering, no wild cards. Input is case-insensitive.

## Architecture

The design is split across three headers and one implementation file:

### `Card` (struct — `inc/card.hpp`)

A plain data type holding a card's numeric rank (2–14, where 11=J, 12=Q, 13=K, 14=A) and suit character. Defines `operator<` for sorting by rank in descending order.

### `HandRank` (enum — `inc/handrank.hpp`)

An enum inside the `HandRankType` namespace with integer values 0–8 representing the nine hand categories in ascending strength:

| Value | Category         |
|-------|------------------|
| 0     | High Card        |
| 1     | One Pair         |
| 2     | Two Pair         |
| 3     | Three of a Kind  |
| 4     | Straight         |
| 5     | Flush            |
| 6     | Full House       |
| 7     | Four of a Kind   |
| 8     | Straight Flush   |

A Royal Flush is represented as a Straight Flush with an Ace-high tiebreaker — no separate category is needed since it is simply the highest possible Straight Flush.

### `PokerHand` (class — `inc/pokerhand.hpp`, `src/pokerhand.cpp`)

**Public interface:**

| Member | Purpose |
|--------|---------|
| `PokerHand(const std::string&)` | Parse the string, store five `Card`s, evaluate the hand |
| `getRank()` | Return the `HandRank` enum value |
| `getRankString()` | Return a human-readable string (e.g. `"Full House"`) |
| `compareTo(const PokerHand&)` | Return -1, 0, or 1 for less-than, equal, greater-than |
| `operator<, >, ==, <=, >=` | Relational operators delegating to `compareTo` |

**Private members:**

| Member | Purpose |
|--------|---------|
| `cards_` | `vector<Card>` — the five cards, sorted descending by rank after parsing |
| `rank_` | The evaluated `HandRank` |
| `tiebreakers_` | `vector<int>` — ordered list of rank values used to break ties between hands of the same category |

## Key Algorithms

### Parsing (`parseHand`)

1. Tokenize the input string on whitespace via `istringstream`.
2. Convert each two-character token into a `Card` (rank char mapped to 2–14, suit validated against C/D/H/S).
3. Reject input that does not produce exactly five cards, or contains invalid rank/suit characters.
4. Sort cards descending by rank.

### Hand Evaluation (`evaluateHand`)

1. Compute two boolean properties:
   - **Flush:** all five cards share the same suit.
   - **Straight:** five consecutive ranks, with a special case for the Ace-low (wheel) straight A-2-3-4-5.

2. Build a frequency map (`std::map<int,int>`) counting how many cards share each rank.

3. Convert to a list of `(count, rank)` pairs sorted by count descending, then rank descending. This ordering drives both classification and tiebreaking.

4. Classify using the standard poker hierarchy:

   ```
   straight && flush        → Straight Flush
   count[0] == 4            → Four of a Kind
   count[0] == 3, count[1] == 2 → Full House
   flush                    → Flush
   straight                 → Straight
   count[0] == 3            → Three of a Kind
   count[0] == 2, count[1] == 2 → Two Pair
   count[0] == 2            → One Pair
   otherwise                → High Card
   ```

5. Store the rank values from the frequency-sorted pairs as `tiebreakers_`. For the Ace-low straight, the top tiebreaker is set to 5 (not 14) so it correctly loses to a 6-high straight.

### Comparison (`compareTo`)

1. Compare `HandRank` enum values first — higher category always wins.
2. If categories are equal, compare `tiebreakers_` vectors element-by-element. The frequency-based ordering ensures the most significant group (e.g., the triple in a full house) is compared before lesser groups or kickers.
3. Return 0 if all tiebreakers match (a tie).

## Testing Strategy

Tests are organized into five groups in `src/main.cpp`:

| Group | What It Covers |
|-------|----------------|
| **Hand Rank Detection** | Each of the 9 categories is correctly identified, including Ace-low straight and Royal Flush |
| **Hand Comparisons** | Cross-category wins, same-category tiebreaking by primary group and kickers for all 9 categories |
| **Operator Tests** | All relational operators (`<`, `>`, `==`, `<=`, `>=`) produce correct boolean results |
| **Edge Cases** | Ace-low straight vs. 6-high straight ordering, flush detection, Royal Flush as Straight Flush, case-insensitive parsing |
| **Error Handling** | Too few/many cards, invalid rank, invalid suit, malformed tokens, empty string — all throw `std::invalid_argument` |

## Build

CMake-based (C++17). Single executable target `cardhand`:

```
mkdir build && cd build
cmake ..
make
./cardhand
```
