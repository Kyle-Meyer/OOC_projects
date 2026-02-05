#include "pokerhand.hpp"
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <cctype>

// Comparator functor for sorting freq pairs
struct CompareFreqPairs
{
   bool operator()(const std::pair<int, int>& a, const std::pair<int, int>& b) const
   {
      if (a.first != b.first) return a.first > b.first;
      return a.second > b.second;
   }
};

PokerHand::PokerHand(const std::string& handStr)
{
   parseHand(handStr);
   evaluateHand();
}

void PokerHand::parseHand(const std::string& handStr)
{
   std::istringstream iss(handStr);
   std::string cardStr;
   
   while (iss >> cardStr) {
      if (cardStr.length() != 2) {
         throw std::invalid_argument("Invalid card format: " + cardStr);
      }
      
      Card card;
      card.rank = charToRank(std::toupper(cardStr[0]));
      card.suit = std::toupper(cardStr[1]);
      
      if (card.suit != 'C' && card.suit != 'D' && 
          card.suit != 'H' && card.suit != 'S') {
         throw std::invalid_argument("Invalid suit: " + std::string(1, cardStr[1]));
      }
      
      cards_.push_back(card);
   }
   
   if (cards_.size() != 5) {
      throw std::invalid_argument("Hand must contain exactly 5 cards");
   }
   
   // Sort cards by rank (descending)
   std::sort(cards_.begin(), cards_.end());
}

int PokerHand::charToRank(char c)
{
   switch (c) {
      case '2': return 2;
      case '3': return 3;
      case '4': return 4;
      case '5': return 5;
      case '6': return 6;
      case '7': return 7;
      case '8': return 8;
      case '9': return 9;
      case 'T': return 10;
      case 'J': return 11;
      case 'Q': return 12;
      case 'K': return 13;
      case 'A': return 14;
      default:
         throw std::invalid_argument("Invalid rank: " + std::string(1, c));
   }
}

bool PokerHand::isFlush() const
{
   char suit = cards_[0].suit;
   for (size_t i = 1; i < cards_.size(); ++i) {
      if (cards_[i].suit != suit) {
         return false;
      }
   }
   return true;
}

bool PokerHand::isStraight() const
{
   // Check for regular straight
   for (size_t i = 0; i < cards_.size() - 1; ++i) {
      if (cards_[i].rank - cards_[i + 1].rank != 1) {
         // Check for ace-low straight (A-2-3-4-5)
         if (i == 0 && cards_[0].rank == 14 && cards_[1].rank == 5 &&
             cards_[2].rank == 4 && cards_[3].rank == 3 && cards_[4].rank == 2) {
            return true;
         }
         return false;
      }
   }
   return true;
}

std::map<int, int> PokerHand::getRankCounts() const
{
   std::map<int, int> counts;
   for (size_t i = 0; i < cards_.size(); ++i) {
      counts[cards_[i].rank]++;
   }
   return counts;
}

void PokerHand::evaluateHand()
{
   bool flush = isFlush();
   bool straight = isStraight();
   std::map<int, int> rankCounts = getRankCounts();
   
   // Get frequency distribution
   std::vector<std::pair<int, int> > freqPairs; // (count, rank)
   for (std::map<int, int>::const_iterator it = rankCounts.begin(); 
        it != rankCounts.end(); ++it) {
      freqPairs.push_back(std::make_pair(it->second, it->first));
   }
   
   // Sort by count (descending), then by rank (descending)
   std::sort(freqPairs.begin(), freqPairs.end(), CompareFreqPairs());
   
   // Build tiebreakers based on frequency
   tiebreakers_.clear();
   for (size_t i = 0; i < freqPairs.size(); ++i) {
      tiebreakers_.push_back(freqPairs[i].second);
   }
   
   // Special case: ace-low straight
   if (straight && cards_[0].rank == 14 && cards_[1].rank == 5) {
      tiebreakers_[0] = 5; // In ace-low straight, highest card is 5
   }
   
   // Determine hand rank
   if (straight && flush) {
      rank_ = HandRankType::StraightFlush;
   } else if (freqPairs[0].first == 4) {
      rank_ = HandRankType::FourOfAKind;
   } else if (freqPairs[0].first == 3 && freqPairs[1].first == 2) {
      rank_ = HandRankType::FullHouse;
   } else if (flush) {
      rank_ = HandRankType::Flush;
   } else if (straight) {
      rank_ = HandRankType::Straight;
   } else if (freqPairs[0].first == 3) {
      rank_ = HandRankType::ThreeOfAKind;
   } else if (freqPairs[0].first == 2 && freqPairs[1].first == 2) {
      rank_ = HandRankType::TwoPair;
   } else if (freqPairs[0].first == 2) {
      rank_ = HandRankType::OnePair;
   } else {
      rank_ = HandRankType::HighCard;
   }
}

int PokerHand::compareTo(const PokerHand& other) const
{
   // First compare by hand rank
   if (rank_ != other.rank_) {
      return (rank_ > other.rank_) ? 1 : -1;
   }
   
   // If same rank, compare tiebreakers
   for (size_t i = 0; i < tiebreakers_.size() && i < other.tiebreakers_.size(); ++i) {
      if (tiebreakers_[i] != other.tiebreakers_[i]) {
         return (tiebreakers_[i] > other.tiebreakers_[i]) ? 1 : -1;
      }
   }
   
   return 0; // Hands are equal
}

std::string PokerHand::getRankString() const
{
   return rankToString(rank_);
}

std::string PokerHand::rankToString(HandRank rank)
{
   switch (rank) {
      case HandRankType::HighCard:      return "High Card";
      case HandRankType::OnePair:       return "One Pair";
      case HandRankType::TwoPair:       return "Two Pair";
      case HandRankType::ThreeOfAKind:  return "Three of a Kind";
      case HandRankType::Straight:      return "Straight";
      case HandRankType::Flush:         return "Flush";
      case HandRankType::FullHouse:     return "Full House";
      case HandRankType::FourOfAKind:   return "Four of a Kind";
      case HandRankType::StraightFlush: return "Straight Flush";
      default:                          return "Unknown";
   }
}
