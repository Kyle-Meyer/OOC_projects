#ifndef POKER_HAND_H
#define POKER_HAND_H

#include "card.hpp"
#include "handrank.hpp"
#include <string>
#include <vector>
#include <map>

class PokerHand 
{
public:
  // Constructor accepting string like "9H 7S TH JS 8D"
  explicit PokerHand(const std::string& handStr);

  // Get the rank of this hand
  HandRank getRank() const { return rank_; }
  
  // Get string representation of the rank
  std::string getRankString() const;

  // Compare two hands (-1 if this < other, 0 if equal, 1 if this > other)
  int compareTo(const PokerHand& other) const;

  // Operator overloads for comparison
  bool operator<(const PokerHand& other) const { return compareTo(other) < 0; }
  bool operator>(const PokerHand& other) const { return compareTo(other) > 0; }
  bool operator==(const PokerHand& other) const { return compareTo(other) == 0; }
  bool operator<=(const PokerHand& other) const { return compareTo(other) <= 0; }
  bool operator>=(const PokerHand& other) const { return compareTo(other) >= 0; }

private:
  std::vector<Card> cards_;
  HandRank rank_;
  std::vector<int> tiebreakers_; // For comparing hands of same rank

  // Parse string into cards
  void parseHand(const std::string& handStr);
  
  // Determine the rank of the hand
  void evaluateHand();
  
  // Helper functions for hand evaluation
  bool isFlush() const;
  bool isStraight() const;
  std::map<int, int> getRankCounts() const;
  
  // Convert rank char to numeric value
  static int charToRank(char c);
  
  // Convert rank enum to string
  static std::string rankToString(HandRank rank);
};

#endif // POKER_HAND_H
