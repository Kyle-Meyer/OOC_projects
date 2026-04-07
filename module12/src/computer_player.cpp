#include "computer_player.hpp"
#include <iostream>
#include <map>
#include <algorithm>

ComputerPlayer::ComputerPlayer(const std::string& name, int chips)
   : Player(name, chips) {}

int ComputerPlayer::placeBet(int toCall, int pot)
{
   PokerHand hand = evaluateHand();
   HandRank  rank = hand.getRank();

   int amount = 0;

   if (rank >= HandRankType::FourOfAKind)
   {
      amount = toCall + 20; // raise big
   }
   else if (rank >= HandRankType::FullHouse)
   {
      amount = toCall + 10; // raise medium
   }
   else if (rank >= HandRankType::ThreeOfAKind)
   {
      amount = toCall; // call
   }
   else if (rank == HandRankType::TwoPair)
   {
      amount = toCall; // call
   }
   else if (rank == HandRankType::OnePair)
   {
      // Call only if the price is reasonable
      if (toCall <= chips_ / 4)
         amount = toCall;
      else if (toCall > 0)
      {
         std::cout << "[" << name_ << "] folds.\n";
         return -1;
      }
   }
   else // HighCard
   {
      if (toCall > 0)
      {
         std::cout << "[" << name_ << "] folds.\n";
         return -1;
      }
      amount = 0; // check
   }

   amount = std::min(amount, chips_);

   if (amount > toCall)
      std::cout << "[" << name_ << "] raises, putting in " << amount << ".\n";
   else if (amount == toCall && toCall > 0)
      std::cout << "[" << name_ << "] calls " << amount << ".\n";
   else
      std::cout << "[" << name_ << "] checks.\n";

   return amount;
}

std::vector<int> ComputerPlayer::chooseDiscards()
{
   PokerHand hand = evaluateHand();
   HandRank  rank = hand.getRank();

   std::map<int,int> counts;
   for (const Card& c : cards_) counts[c.rank]++;

   std::vector<int> discards;

   if (rank == HandRankType::StraightFlush ||
       rank == HandRankType::FourOfAKind   ||
       rank == HandRankType::FullHouse     ||
       rank == HandRankType::Flush         ||
       rank == HandRankType::Straight)
   {
      // Keep everything
   }
   else if (rank == HandRankType::ThreeOfAKind)
   {
      int tripleRank = -1;
      for (auto& p : counts) if (p.second == 3) tripleRank = p.first;
      for (int i = 0; i < 5; ++i)
         if (cards_[i].rank != tripleRank) discards.push_back(i);
   }
   else if (rank == HandRankType::TwoPair)
   {
      // Discard the one kicker
      for (int i = 0; i < 5; ++i)
         if (counts[cards_[i].rank] == 1) discards.push_back(i);
   }
   else if (rank == HandRankType::OnePair)
   {
      int pairRank = -1;
      for (auto& p : counts) if (p.second == 2) pairRank = p.first;
      for (int i = 0; i < 5; ++i)
         if (cards_[i].rank != pairRank) discards.push_back(i);
   }
   else // HighCard: keep top 2 cards by rank
   {
      std::vector<std::pair<int,int>> byRank; // (rank, index)
      for (int i = 0; i < 5; ++i)
         byRank.push_back({cards_[i].rank, i});
      std::sort(byRank.begin(), byRank.end(),
                [](const std::pair<int,int>& a, const std::pair<int,int>& b)
                { return a.first > b.first; });
      for (int i = 2; i < 5; ++i)
         discards.push_back(byRank[i].second);
   }

   if (discards.empty())
      std::cout << "[" << name_ << "] keeps all cards.\n";
   else
      std::cout << "[" << name_ << "] discards " << discards.size() << " card(s).\n";

   return discards;
}
