#ifndef GAME_H
#define GAME_H

#include "deck.hpp"
#include "player.hpp"
#include <vector>
#include <memory>
#include <string>

class Game
{
public:
   Game(std::vector<std::unique_ptr<Player>> players, int ante);
   void run();

private:
   void playRound();
   void collectAntes();
   void dealHands();
   void bettingRound(const std::string& label);
   void drawPhase();
   void showdown();
   void printStatus() const;
   int  countActive() const; // non-folded players with a hand

   std::vector<std::unique_ptr<Player>> players_;
   Deck deck_;
   int  pot_;
   int  ante_;
   int  roundNum_;
};

#endif // GAME_H
