#include "deck.hpp"
#include <stdexcept>

Deck::Deck() : rng_(std::random_device{}())
{
   build();
}

void Deck::build()
{
   cards_.clear();
   const char suits[] = {'C', 'D', 'H', 'S'};
   for (char suit : suits)
      for (int rank = 2; rank <= 14; ++rank)
         cards_.push_back({rank, suit});
}

void Deck::shuffle()
{
   for (int i = static_cast<int>(cards_.size()) - 1; i > 0; --i)
   {
      std::uniform_int_distribution<int> dist(0, i);
      int j = dist(rng_);
      std::swap(cards_[i], cards_[j]);
   }
}

Card Deck::deal()
{
   if (cards_.empty())
      throw std::runtime_error("Deck is empty");
   Card c = cards_.back();
   cards_.pop_back();
   return c;
}

void Deck::reset()
{
   build();
   shuffle();
}

int Deck::remaining() const
{
   return static_cast<int>(cards_.size());
}
