#ifndef DECK_H
#define DECK_H

#include "card.hpp"
#include <vector>
#include <random>

class Deck
{
public:
   Deck();

   void shuffle();   // Fisher-Yates in-place
   Card deal();      // remove and return top card
   void reset();     // rebuild full deck and shuffle
   int  remaining() const;

private:
   std::vector<Card> cards_;
   std::mt19937      rng_;

   void build();
};

#endif // DECK_H
