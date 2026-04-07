#include "player.hpp"
#include <algorithm>

Player::Player(const std::string& name, int chips)
   : name_(name), chips_(chips), folded_(false) {}

void Player::receiveCard(const Card& c) { cards_.push_back(c); }

void Player::discardCards(const std::vector<int>& indices)
{
   // Erase from highest index downward to avoid index shifting
   std::vector<int> sorted = indices;
   std::sort(sorted.begin(), sorted.end(), std::greater<int>());
   for (int i : sorted)
      if (i >= 0 && i < static_cast<int>(cards_.size()))
         cards_.erase(cards_.begin() + i);
}

void Player::resetRound()
{
   cards_.clear();
   folded_ = false;
}

void Player::fold() { folded_ = true; }

void Player::addChips(int amount) { chips_ += amount; }

void Player::deductChips(int amount)
{
   chips_ -= amount;
   if (chips_ < 0) chips_ = 0;
}

PokerHand Player::evaluateHand() const { return PokerHand(cards_); }
