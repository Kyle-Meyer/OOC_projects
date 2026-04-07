#ifndef COMPUTER_PLAYER_H
#define COMPUTER_PLAYER_H

#include "player.hpp"

class ComputerPlayer : public Player
{
public:
   ComputerPlayer(const std::string& name, int chips);

   int              placeBet(int toCall, int pot) override;
   std::vector<int> chooseDiscards()              override;
};

#endif // COMPUTER_PLAYER_H
