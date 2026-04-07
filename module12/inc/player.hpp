#ifndef PLAYER_H
#define PLAYER_H

#include "card.hpp"
#include "pokerhand.hpp"
#include <string>
#include <vector>

class Player
{
public:
   Player(const std::string& name, int chips);
   virtual ~Player() = default;

   // Called each betting round. toCall is the amount needed to match the current
   // highest bet. Returns the amount added to the pot, or -1 to fold.
   virtual int placeBet(int toCall, int pot) = 0;

   // Returns indices (0-4) of cards to discard during the draw phase.
   virtual std::vector<int> chooseDiscards() = 0;

   void receiveCard(const Card& c);
   void discardCards(const std::vector<int>& indices);
   void resetRound();        // clear hand and un-fold for a new round
   void fold();
   void addChips(int amount);
   void deductChips(int amount); // clamps to 0; caller verifies sufficiency

   bool                    hasFolded() const { return folded_; }
   const std::string&      getName()   const { return name_; }
   int                     getChips()  const { return chips_; }
   const std::vector<Card>& getHand()  const { return cards_; }
   PokerHand               evaluateHand() const;

protected:
   std::string       name_;
   int               chips_;
   bool              folded_;
   std::vector<Card> cards_;
};

#endif // PLAYER_H
