#ifndef CARD_H
#define CARD_H

struct Card 
{
   int rank;  // 2-14 (where 11=J, 12=Q, 13=K, 14=A)
   char suit; // C, D, H, S
    
   bool operator<(const Card& other) const 
   {
      return rank > other.rank;
   }
};

#endif // CARD_H
