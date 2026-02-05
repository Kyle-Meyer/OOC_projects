#include <iostream>
#include <vector>
#include <string>
#include "pokerhand.hpp"

struct TestCase
{
   std::string hand1;
   std::string hand2;
   std::string expectedWinner; // "hand1", "hand2", or "tie"
   std::string description;
};

void runTest(const TestCase& test)
{
   try {
      PokerHand h1(test.hand1);
      PokerHand h2(test.hand2);
      
      std::cout << "Test: " << test.description << std::endl;
      std::cout << "  Hand 1: " << test.hand1 << " (" << h1.getRankString() << ")" << std::endl;
      std::cout << "  Hand 2: " << test.hand2 << " (" << h2.getRankString() << ")" << std::endl;
      
      std::string actualWinner;
      if (h1 > h2) {
         actualWinner = "hand1";
         std::cout << "  Result: Hand 1 wins" << std::endl;
      } else if (h2 > h1) {
         actualWinner = "hand2";
         std::cout << "  Result: Hand 2 wins" << std::endl;
      } else {
         actualWinner = "tie";
         std::cout << "  Result: Tie" << std::endl;
      }
      
      if (actualWinner == test.expectedWinner) {
         std::cout << "  PASS" << std::endl;
      } else {
         std::cout << "  FAIL (expected " << test.expectedWinner << ")" << std::endl;
      }
      std::cout << std::endl;
      
   } catch (const std::exception& e) {
      std::cout << "  ERROR: " << e.what() << std::endl << std::endl;
   }
}

void testHandRankDetection()
{
   std::cout << "=== Testing Hand Rank Detection ===" << std::endl << std::endl;
   
   struct RankTest
   {
      std::string hand;
      std::string expectedRank;
   };
   
   std::vector<RankTest> tests;
   
   RankTest t1 = {"2H 3D 5S 9C KD", "High Card"};
   tests.push_back(t1);
   
   RankTest t2 = {"2H 2D 5S 9C KD", "One Pair"};
   tests.push_back(t2);
   
   RankTest t3 = {"2H 2D 5S 5C KD", "Two Pair"};
   tests.push_back(t3);
   
   RankTest t4 = {"2H 2D 2S 9C KD", "Three of a Kind"};
   tests.push_back(t4);
   
   RankTest t5 = {"2H 3D 4S 5C 6D", "Straight"};
   tests.push_back(t5);
   
   RankTest t6 = {"AH 2D 3S 4C 5D", "Straight"};
   tests.push_back(t6);
   
   RankTest t7 = {"2H 5H 9H QH KH", "Flush"};
   tests.push_back(t7);
   
   RankTest t8 = {"2H 2D 2S 9C 9D", "Full House"};
   tests.push_back(t8);
   
   RankTest t9 = {"2H 2D 2S 2C KD", "Four of a Kind"};
   tests.push_back(t9);
   
   RankTest t10 = {"2H 3H 4H 5H 6H", "Straight Flush"};
   tests.push_back(t10);
   
   RankTest t11 = {"TH JH QH KH AH", "Straight Flush"};
   tests.push_back(t11);
   
   for (size_t i = 0; i < tests.size(); ++i) {
      try {
         PokerHand hand(tests[i].hand);
         std::string rank = hand.getRankString();
         
         std::cout << "Hand: " << tests[i].hand << std::endl;
         std::cout << "  Expected: " << tests[i].expectedRank << std::endl;
         std::cout << "  Got:      " << rank << std::endl;
         
         if (rank == tests[i].expectedRank) {
            std::cout << "  PASS" << std::endl;
         } else {
            std::cout << "  FAIL" << std::endl;
         }
         std::cout << std::endl;
         
      } catch (const std::exception& e) {
         std::cout << "  ERROR: " << e.what() << std::endl << std::endl;
      }
   }
}

void testHandComparisons()
{
   std::cout << "=== Testing Hand Comparisons ===" << std::endl << std::endl;
   
   std::vector<TestCase> tests;
   
   // Different rank categories
   TestCase tc1 = {"2C 3D 4S 5H 6C", "AC AD AH AS KC", "hand1", 
                   "Straight beats Four of a Kind? No - Four of a Kind wins"};
   tests.push_back(tc1);
   
   TestCase tc2 = {"AC AD AH AS KC", "2C 3D 4S 5H 6C", "hand1", 
                   "Four of a Kind beats Straight"};
   tests.push_back(tc2);
   
   TestCase tc3 = {"2H 2D 2S 9C 9D", "2C 3C 4C 5C 7C", "hand2", 
                   "Flush beats Full House? No - Full House wins"};
   tests.push_back(tc3);
   
   TestCase tc4 = {"2C 3C 4C 5C 7C", "2H 2D 2S 9C 9D", "hand2", 
                   "Full House beats Flush"};
   tests.push_back(tc4);
   
   // High card comparisons
   TestCase tc5 = {"2H 3D 5S 9C KD", "2C 3S 5D 9H AD", "hand2", 
                   "High Card: Ace high beats King high"};
   tests.push_back(tc5);
   
   TestCase tc6 = {"2H 3D 5S 9C AD", "2C 3S 5D 9H KD", "hand1", 
                   "High Card: Ace high beats King high"};
   tests.push_back(tc6);
   
   TestCase tc7 = {"2H 3D 5S 9C KD", "2C 3S 5D 8H KH", "hand1", 
                   "High Card: K-9 beats K-8 (kicker)"};
   tests.push_back(tc7);
   
   // One pair comparisons
   TestCase tc8 = {"2H 2D 5S 9C KD", "3H 3D 5C 9S AD", "hand2", 
                   "One Pair: Pair of 3s beats pair of 2s"};
   tests.push_back(tc8);
   
   TestCase tc9 = {"KH KD 5S 9C 2D", "KC KS 5C 9H 3D", "hand2", 
                   "One Pair: Same pair, 3 kicker beats 2 kicker"};
   tests.push_back(tc9);
   
   TestCase tc10 = {"AH AD QS JC 9D", "AC AS QC JH 8D", "hand1", 
                    "One Pair: Aces with 9 kicker beats Aces with 8 kicker"};
   tests.push_back(tc10);
   
   // Two pair comparisons
   TestCase tc11 = {"KH KD 5S 5C 2D", "KC KS 6C 6H 3D", "hand2", 
                    "Two Pair: K-6 beats K-5"};
   tests.push_back(tc11);
   
   TestCase tc12 = {"AH AD KS KC QD", "AC AS KH KD JD", "hand1", 
                    "Two Pair: A-K with Q kicker beats A-K with J kicker"};
   tests.push_back(tc12);
   
   // Three of a kind comparisons
   TestCase tc13 = {"5H 5D 5S 9C KD", "6H 6D 6C 2S 3D", "hand2", 
                    "Three of a Kind: Trip 6s beats trip 5s"};
   tests.push_back(tc13);
   
   TestCase tc14 = {"AH AD AS KC QD", "AC AH AD KS JD", "hand1", 
                    "Three of a Kind: Trip Aces, Q kicker beats J kicker"};
   tests.push_back(tc14);
   
   // Straight comparisons
   TestCase tc15 = {"2H 3D 4S 5C 6D", "3H 4D 5S 6C 7D", "hand2", 
                    "Straight: 7-high beats 6-high"};
   tests.push_back(tc15);
   
   TestCase tc16 = {"TH JD QS KC AD", "9H TD JS QC KD", "hand1", 
                    "Straight: Ace-high beats King-high"};
   tests.push_back(tc16);
   
   TestCase tc17 = {"AH 2D 3S 4C 5D", "2H 3D 4S 5C 6D", "hand2", 
                    "Straight: 6-high beats 5-high (ace-low)"};
   tests.push_back(tc17);
   
   // Flush comparisons
   TestCase tc18 = {"2H 3H 5H 9H KH", "2D 3D 5D 9D AD", "hand2", 
                    "Flush: Ace-high beats King-high"};
   tests.push_back(tc18);
   
   TestCase tc19 = {"2H 3H 5H 9H KH", "2D 3D 5D 8D KD", "hand1", 
                    "Flush: K-9 beats K-8"};
   tests.push_back(tc19);
   
   // Full house comparisons
   TestCase tc20 = {"5H 5D 5S 9C 9D", "6H 6D 6C 2S 2D", "hand2", 
                    "Full House: 6s full of 2s beats 5s full of 9s"};
   tests.push_back(tc20);
   
   TestCase tc21 = {"KH KD KS 5C 5D", "KC KH KD 6S 6C", "hand2", 
                    "Full House: Kings full of 6s beats Kings full of 5s"};
   tests.push_back(tc21);
   
   // Four of a kind comparisons
   TestCase tc22 = {"5H 5D 5S 5C KD", "6H 6D 6C 6S 2D", "hand2", 
                    "Four of a Kind: Quad 6s beats quad 5s"};
   tests.push_back(tc22);
   
   TestCase tc23 = {"AH AD AS AC KD", "AC AH AD AS QD", "hand1", 
                    "Four of a Kind: Quad Aces, K kicker beats Q kicker"};
   tests.push_back(tc23);
   
   // Straight flush comparisons
   TestCase tc24 = {"2H 3H 4H 5H 6H", "3D 4D 5D 6D 7D", "hand2", 
                    "Straight Flush: 7-high beats 6-high"};
   tests.push_back(tc24);
   
   TestCase tc25 = {"TH JH QH KH AH", "9D TD JD QD KD", "hand1", 
                    "Straight Flush: Royal flush beats King-high"};
   tests.push_back(tc25);
   
   // Tie cases
   TestCase tc26 = {"2H 3D 5S 9C KD", "2C 3S 5D 9H KH", "tie", 
                    "Tie: Identical high cards"};
   tests.push_back(tc26);
   
   TestCase tc27 = {"AH AD 5S 9C KD", "AC AS 5C 9H KH", "tie", 
                    "Tie: Identical pairs and kickers"};
   tests.push_back(tc27);
   
   for (size_t i = 0; i < tests.size(); ++i) {
      runTest(tests[i]);
   }
}

void testErrorHandling()
{
   std::cout << "=== Testing Error Handling ===" << std::endl << std::endl;
   
   std::vector<std::string> invalidHands;
   invalidHands.push_back("2H 3D 5S 9C");        // Too few cards
   invalidHands.push_back("2H 3D 5S 9C KD QH");  // Too many cards
   invalidHands.push_back("2H 3D 5S 9C XD");     // Invalid rank
   invalidHands.push_back("2H 3D 5S 9C KX");     // Invalid suit
   invalidHands.push_back("2H3D 5S 9C KD");      // Missing space
   invalidHands.push_back("");                   // Empty string
   
   for (size_t i = 0; i < invalidHands.size(); ++i) {
      std::cout << "Testing invalid hand: \"" << invalidHands[i] << "\"" << std::endl;
      try {
         PokerHand hand(invalidHands[i]);
         std::cout << "  FAIL: Should have thrown exception" << std::endl;
      } catch (const std::exception& e) {
         std::cout << "  PASS: Caught exception: " << e.what() << std::endl;
      }
      std::cout << std::endl;
   }
}

void testOperators()
{
   std::cout << "=== Testing Comparison Operators ===" << std::endl << std::endl;
   
   PokerHand high("2H 3D 5S 9C KD");        // High card
   PokerHand pair("2H 2D 5S 9C KD");        // One pair
   PokerHand samePair("2C 2S 5D 9H KH");    // Same one pair
   
   std::cout << "high < pair: " << (high < pair ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << "pair > high: " << (pair > high ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << "pair == samePair: " << (pair == samePair ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << "pair != high: " << (!(pair == high) ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << "high <= pair: " << (high <= pair ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << "pair >= samePair: " << (pair >= samePair ? "true" : "false") << " (expected: true)" << std::endl;
   std::cout << std::endl;
}

void testEdgeCases()
{
   std::cout << "=== Testing Edge Cases ===" << std::endl << std::endl;
   
   // Test all same suit (flush) vs different suits
   std::cout << "Test: Flush detection" << std::endl;
   PokerHand flush("2H 5H 9H QH KH");
   std::cout << "  2H 5H 9H QH KH: " << flush.getRankString() << " (expected: Flush)" << std::endl;
   std::cout << std::endl;
   
   // Test ace-low straight
   std::cout << "Test: Ace-low straight (wheel)" << std::endl;
   PokerHand wheel("AH 2D 3S 4C 5D");
   std::cout << "  AH 2D 3S 4C 5D: " << wheel.getRankString() << " (expected: Straight)" << std::endl;
   
   // Verify ace-low straight loses to 6-high straight
   PokerHand sixHigh("2H 3D 4S 5C 6D");
   if (wheel < sixHigh) {
      std::cout << "  PASS: Ace-low straight correctly loses to 6-high straight" << std::endl;
   } else {
      std::cout << "  FAIL: Ace-low straight should lose to 6-high straight" << std::endl;
   }
   std::cout << std::endl;
   
   // Test royal flush
   std::cout << "Test: Royal flush" << std::endl;
   PokerHand royal("TH JH QH KH AH");
   std::cout << "  TH JH QH KH AH: " << royal.getRankString() << " (expected: Straight Flush)" << std::endl;
   std::cout << std::endl;
   
   // Test case insensitivity
   std::cout << "Test: Case insensitivity" << std::endl;
   PokerHand lower("2h 3d 5s 9c kd");
   PokerHand upper("2H 3D 5S 9C KD");
   if (lower == upper) {
      std::cout << "  PASS: Case insensitive parsing works" << std::endl;
   } else {
      std::cout << "  FAIL: Case sensitivity issue" << std::endl;
   }
   std::cout << std::endl;
}

int main()
{
   std::cout << "========================================" << std::endl;
   std::cout << "  POKER HAND EVALUATION TEST SUITE" << std::endl;
   std::cout << "========================================" << std::endl << std::endl;
   
   testHandRankDetection();
   testHandComparisons();
   testOperators();
   testEdgeCases();
   testErrorHandling();
   
   std::cout << "========================================" << std::endl;
   std::cout << "  ALL TESTS COMPLETE" << std::endl;
   std::cout << "========================================" << std::endl;
   
   return 0;
}
