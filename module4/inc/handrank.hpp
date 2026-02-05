#ifndef HAND_RANK_H
#define HAND_RANK_H

namespace HandRankType 
{
   enum HandRank 
   {
      HighCard = 0,
      OnePair,
      TwoPair,
      ThreeOfAKind,
      Straight,
      Flush,
      FullHouse,
      FourOfAKind,
      StraightFlush
   };
}

typedef HandRankType::HandRank HandRank;

#endif // HAND_RANK_H
