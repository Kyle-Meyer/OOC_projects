#ifndef CARD_H
#define CARD_H

#include <string>

struct Card
{
   int rank;  // 2-14 (where 11=J, 12=Q, 13=K, 14=A)
   char suit; // C, D, H, S

   bool operator<(const Card& other) const
   {
      return rank > other.rank; // descending sort
   }
};

inline std::string cardToString(const Card& c)
{
   std::string s;
   if      (c.rank == 14) s += 'A';
   else if (c.rank == 13) s += 'K';
   else if (c.rank == 12) s += 'Q';
   else if (c.rank == 11) s += 'J';
   else if (c.rank == 10) s += 'T';
   else                   s += static_cast<char>('0' + c.rank);
   s += c.suit;
   return s;
}

#endif // CARD_H
