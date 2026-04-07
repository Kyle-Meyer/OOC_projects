#include "human_player.hpp"
#include <iostream>
#include <sstream>
#include <algorithm>
#include <cctype>

HumanPlayer::HumanPlayer(const std::string& name, int chips)
   : Player(name, chips) {}

int HumanPlayer::placeBet(int toCall, int pot)
{
   std::cout << "\n[" << name_ << "] Chips: " << chips_
             << "  |  Pot: " << pot;
   if (toCall > 0)
      std::cout << "  |  To call: " << toCall;
   std::cout << "\n";

   std::cout << "Hand: ";
   for (const Card& c : cards_) std::cout << cardToString(c) << " ";
   std::cout << "\n";

   while (true)
   {
      if (toCall == 0)
         std::cout << "Action: (c)heck  (r)aise <amount>  (f)old\n> ";
      else
         std::cout << "Action: (c)all " << std::min(toCall, chips_)
                   << "  (r)aise <amount>  (f)old\n> ";

      std::string line;
      std::getline(std::cin, line);
      if (line.empty()) continue;

      char ch = static_cast<char>(std::tolower(static_cast<unsigned char>(line[0])));

      if (ch == 'f')
      {
         std::cout << name_ << " folds.\n";
         return -1;
      }
      else if (ch == 'c')
      {
         int actual = std::min(toCall, chips_);
         std::cout << name_ << (toCall == 0 ? " checks.\n" : " calls " + std::to_string(actual) + ".\n");
         return actual;
      }
      else if (ch == 'r')
      {
         std::istringstream iss(line.substr(1));
         int raiseBy = 0;
         iss >> raiseBy;
         if (raiseBy <= 0)
         {
            std::cout << "Enter a positive raise amount after 'r'. Try again.\n";
            continue;
         }
         int total = std::min(toCall + raiseBy, chips_);
         std::cout << name_ << " raises, putting in " << total << ".\n";
         return total;
      }
      else
      {
         std::cout << "Unknown action. Use c, r <amount>, or f.\n";
      }
   }
}

std::vector<int> HumanPlayer::chooseDiscards()
{
   std::cout << "\n[" << name_ << "] Your hand:\n";
   for (int i = 0; i < static_cast<int>(cards_.size()); ++i)
      std::cout << "  [" << i << "] " << cardToString(cards_[i]) << "\n";

   std::cout << "Indices to discard (e.g. '0 3 4'), or Enter to keep all:\n> ";

   std::string line;
   std::getline(std::cin, line);

   std::vector<int> discards;
   std::istringstream iss(line);
   int idx;
   while (iss >> idx)
      if (idx >= 0 && idx < static_cast<int>(cards_.size()))
         discards.push_back(idx);

   // Remove duplicates
   std::sort(discards.begin(), discards.end());
   discards.erase(std::unique(discards.begin(), discards.end()), discards.end());

   std::cout << name_ << " discards " << discards.size() << " card(s).\n";
   return discards;
}
