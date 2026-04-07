#include "game.hpp"
#include <iostream>
#include <algorithm>

Game::Game(std::vector<std::unique_ptr<Player>> players, int ante)
   : players_(std::move(players)), pot_(0), ante_(ante), roundNum_(0) {}

// ---------------------------------------------------------------------------
// Public
// ---------------------------------------------------------------------------

void Game::run()
{
   std::cout << "\n=== Welcome to 5-Card Draw Poker! ===\n";
   printStatus();

   while (true)
   {
      int active = 0;
      for (auto& p : players_)
         if (p->getChips() > 0) ++active;
      if (active <= 1) break;

      playRound();
   }

   // Declare the chip leader as overall winner
   int     maxChips = -1;
   Player* winner   = nullptr;
   for (auto& p : players_)
   {
      if (p->getChips() > maxChips)
      {
         maxChips = p->getChips();
         winner   = p.get();
      }
   }
   std::cout << "\n=== Game Over! " << winner->getName()
             << " wins with " << maxChips << " chips! ===\n";
}

// ---------------------------------------------------------------------------
// Private
// ---------------------------------------------------------------------------

void Game::playRound()
{
   ++roundNum_;
   std::cout << "\n========== Round " << roundNum_ << " ==========\n";

   // Reset all players; immediately fold anyone who can't ante
   for (auto& p : players_)
   {
      p->resetRound();
      if (p->getChips() <= 0)
         p->fold(); // sit out this round
   }

   deck_.reset();
   pot_ = 0;

   collectAntes();
   dealHands();

   bettingRound("Pre-draw betting");
   if (countActive() <= 1) { showdown(); return; }

   drawPhase();

   bettingRound("Post-draw betting");
   showdown();
}

void Game::collectAntes()
{
   std::cout << "\n-- Antes (" << ante_ << " each) --\n";
   for (auto& p : players_)
   {
      if (p->hasFolded()) continue;
      int paid = std::min(ante_, p->getChips());
      p->deductChips(paid);
      pot_ += paid;
      std::cout << "  " << p->getName() << " antes " << paid << ".\n";
   }
   std::cout << "  Pot: " << pot_ << "\n";
}

void Game::dealHands()
{
   std::cout << "\n-- Dealing --\n";
   // One card at a time per player, 5 rounds
   for (int round = 0; round < 5; ++round)
      for (auto& p : players_)
         if (!p->hasFolded())
            p->receiveCard(deck_.deal());
}

void Game::bettingRound(const std::string& label)
{
   std::cout << "\n-- " << label << " (Pot: " << pot_ << ") --\n";

   int n = static_cast<int>(players_.size());
   std::vector<int>  roundBets(n, 0);
   int               highestBet = 0;

   // Everyone who isn't folded and has chips needs to act
   std::vector<bool> needsToAct(n, false);
   for (int i = 0; i < n; ++i)
      if (!players_[i]->hasFolded() && players_[i]->getChips() > 0)
         needsToAct[i] = true;

   bool acted;
   do
   {
      acted = false;
      for (int i = 0; i < n; ++i)
      {
         if (!needsToAct[i]) continue;
         if (players_[i]->hasFolded()) { needsToAct[i] = false; continue; }

         needsToAct[i] = false;
         acted = true;

         int toCall = std::min(highestBet - roundBets[i], players_[i]->getChips());
         int added  = players_[i]->placeBet(toCall, pot_);

         if (added < 0) // fold
         {
            players_[i]->fold();
            if (countActive() <= 1) return;
         }
         else
         {
            added = std::min(added, players_[i]->getChips());
            players_[i]->deductChips(added);
            pot_         += added;
            roundBets[i] += added;

            if (roundBets[i] > highestBet)
            {
               highestBet = roundBets[i];
               // Everyone else needs to respond to the raise
               for (int j = 0; j < n; ++j)
                  if (j != i && !players_[j]->hasFolded() && players_[j]->getChips() > 0)
                     needsToAct[j] = true;
            }
         }
      }
   } while (acted);

   std::cout << "  Pot after betting: " << pot_ << "\n";
}

void Game::drawPhase()
{
   std::cout << "\n-- Draw Phase --\n";
   for (auto& p : players_)
   {
      if (p->hasFolded()) continue;

      std::vector<int> discards = p->chooseDiscards();

      // Don't draw more cards than the deck has remaining
      int canDraw = std::min(static_cast<int>(discards.size()), deck_.remaining());
      if (canDraw < static_cast<int>(discards.size()))
      {
         std::cout << "  (Deck running low — " << p->getName()
                   << " can only draw " << canDraw << " card(s).)\n";
         discards.resize(canDraw);
      }

      p->discardCards(discards);
      for (int i = 0; i < canDraw; ++i)
         p->receiveCard(deck_.deal());
   }
}

void Game::showdown()
{
   std::cout << "\n-- Showdown --\n";

   // Collect non-folded players who actually have a hand
   std::vector<int> active;
   for (int i = 0; i < static_cast<int>(players_.size()); ++i)
      if (!players_[i]->hasFolded() && !players_[i]->getHand().empty())
         active.push_back(i);

   if (active.empty()) return;

   if (active.size() == 1)
   {
      players_[active[0]]->addChips(pot_);
      std::cout << "  " << players_[active[0]]->getName()
                << " wins " << pot_ << " chips (everyone else folded).\n";
      pot_ = 0;
      printStatus();
      return;
   }

   // Reveal hands
   for (int i : active)
   {
      PokerHand h = players_[i]->evaluateHand();
      std::cout << "  " << players_[i]->getName() << ": ";
      for (const Card& c : players_[i]->getHand())
         std::cout << cardToString(c) << " ";
      std::cout << "(" << h.getRankString() << ")\n";
   }

   // Determine winner(s)
   PokerHand        best    = players_[active[0]]->evaluateHand();
   std::vector<int> winners = {active[0]};

   for (int k = 1; k < static_cast<int>(active.size()); ++k)
   {
      int       i   = active[k];
      PokerHand h   = players_[i]->evaluateHand();
      int       cmp = h.compareTo(best);
      if (cmp > 0)      { best = h; winners = {i}; }
      else if (cmp == 0)  winners.push_back(i);
   }

   // Split pot (remainder goes to the first winner)
   int share     = pot_ / static_cast<int>(winners.size());
   int remainder = pot_ % static_cast<int>(winners.size());
   for (int w : winners) players_[w]->addChips(share);
   if (remainder > 0) players_[winners[0]]->addChips(remainder);

   std::cout << "\n  Winner(s): ";
   for (int w : winners) std::cout << players_[w]->getName() << " ";
   std::cout << "— wins " << share << " chip(s) each";
   if (remainder > 0) std::cout << " (+" << remainder << " remainder to " << players_[winners[0]]->getName() << ")";
   std::cout << "!\n";

   pot_ = 0;
   printStatus();
}

void Game::printStatus() const
{
   std::cout << "\n  Chip counts:\n";
   for (auto& p : players_)
      std::cout << "    " << p->getName() << ": " << p->getChips() << "\n";
}

int Game::countActive() const
{
   int count = 0;
   for (auto& p : players_)
      if (!p->hasFolded()) ++count;
   return count;
}
