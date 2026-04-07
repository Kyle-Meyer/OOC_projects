#ifndef HUMAN_PLAYER_H
#define HUMAN_PLAYER_H

#include "player.hpp"

class HumanPlayer : public Player
{
public:
   HumanPlayer(const std::string& name, int chips);

   int              placeBet(int toCall, int pot) override;
   std::vector<int> chooseDiscards()              override;
};

#endif // HUMAN_PLAYER_H
