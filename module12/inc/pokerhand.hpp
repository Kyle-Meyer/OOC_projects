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
   // Construct from a string like "9H 7S TH JS 8D"
   explicit PokerHand(const std::string& handStr);

   // Construct directly from a vector of 5 cards (used in-game)
   explicit PokerHand(const std::vector<Card>& cards);

   HandRank    getRank()       const { return rank_; }
   std::string getRankString() const;

   // Returns -1 / 0 / 1 (less / equal / greater)
   int compareTo(const PokerHand& other) const;

   bool operator<(const PokerHand& other)  const { return compareTo(other) < 0; }
   bool operator>(const PokerHand& other)  const { return compareTo(other) > 0; }
   bool operator==(const PokerHand& other) const { return compareTo(other) == 0; }
   bool operator<=(const PokerHand& other) const { return compareTo(other) <= 0; }
   bool operator>=(const PokerHand& other) const { return compareTo(other) >= 0; }

private:
   std::vector<Card> cards_;
   HandRank          rank_;
   std::vector<int>  tiebreakers_;

   void parseHand(const std::string& handStr);
   void evaluateHand();

   bool             isFlush()      const;
   bool             isStraight()   const;
   std::map<int,int> getRankCounts() const;

   static int         charToRank(char c);
   static std::string rankToString(HandRank rank);
};

#endif // POKER_HAND_H
