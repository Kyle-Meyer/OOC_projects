Create a program that plays 5 card draw Poker . The program should manage a game by
-   Shuffling the deck – see Fisher–Yates shuffle
-   Dealing the cards one at a time to each player
-   Recording the bets
-   And declaring the winner

The game may be configured to include from two to seven players. Each player is either a human or a computerized player. Human players interact via input and output.  Betting can be implemented in any way you prefer.
The computerized player can implement any strategy. It need not be sophisticated; but it must follow the rules. Do not waste time on the strategy unless you have completed all other aspects of the assignment.
You must use your possibly modified classes which apply to this assignment from Assignment #4.
Extra Credit up to 10% - Implement a GUI interface.

The goal is to write a program capable of comparing any poker hand and test it using the following hands 

To simplify exactly what hands are to be compared, compare each of the example pairs and declare a winner

For Royal Flush compare 

A♠ K♠ Q♠ J♠ 10♠ vs 8♥ 7♥ 6♥ 5♥ 4♥ 

For Straight Flush compare

10♣ 9♣ 8♣ 7♣ 6♣ ranks higher than 8♥ 7♥ 6♥ 5♥ 4♥

8♥ 7♥ 6♥ 5♥ 4♥ ranks higher than 6♠ 5♠ 4♠ 3♠ 2♠

7♦ 6♦ 5♦ 4♦ 3♦ and 7♠ 6♠ 5♠ 4♠ 3♠, are of equal rank

Four of a kind

K♠ K♥ K♣ K♦ 3♥ ranks higher than 7♥ 7♦ 7♠ 7♣ Q♥

7♥ 7♦ 7♠ 7♣ Q♥ ranks higher than 7♥ 7♦ 7♠ 7♣ 10♠

4♣ 4♠ 4♦ 4♥ 9♣ and 4♣ 4♠ 4♦ 4♥ 9♦ are of equal rank


         
Full house

       8♠ 8♦ 8♥ 7♦ 7♣ ranks higher than 4♦ 4♠ 4♣ 9♦ 9♣


       4♦ 4♠ 4♣ 9♦ 9♣ ranks higher than 4♦ 4♠ 4♣ 5♣ 5♦

       K♣ K♠ K♦ J♣ J♠ and K♣ K♥ K♦ J♣ J♥, are of equal rank

        Flush

           K♦ J♦ 9♦ 6♦ 4♦ ranks higher than Q♣ J♣ 7♣ 6♣ 5♣

           Q♣ J♣ 7♣ 6♣ 5♣ ranks higher than J♥ 10♥ 9♥ 4♥ 2♥

            J♥ 10♥ 9♥ 4♥ 2♥ ranks higher than J♠ 10♠ 8♠ 6♠ 3♠

            J♠ 10♠ 8♠ 6♠ 3♠ ranks higher than J♥ 10♥ 8♥ 4♥ 3♥

            J♥ 10♥ 8♥ 4♥ 3♥ ranks higher than J♣ 10♣ 8♣ 4♣ 2♣

            10♦ 8♦ 7♦ 6♦ 5♦ and 10♠ 8♠ 7♠ 6♠ 5♠, are of equal rank

    Straight

         J♥ 10♥ 9♣ 8♠ 7♥ ranks higher than 10♠ 9♠ 8♣ 7♥ 6♠

         10♠ 9♠ 8♣ 7♥ 6♠, which ranks higher than 6♣ 5♠ 4♥ 3♠ 2♦

         9♣ 8♣ 7♣ 6♦ 5♦ and 9♠ 8♠ 7♠ 6♥ 5♥, are of equal rank

   Three of a kind

         6♥ 6♦ 6♠ Q♣ 4♠ ranks higher than 3♦ 3♠ 3♣ K♠ 2♠

         3♦ 3♠ 3♣ K♠ 2♠ ranks higher than 3♦ 3♠ 3♣ J♣ 7♥

         3♦ 3♠ 3♣ J♣ 7♥ ranks higher than 3♦ 3♠ 3♣ J♠ 5♦

         9♠ 9♥ 9♦ 10♦ 8♥ and 9♣ 9♠ 9♥ 10♦ 8♦, are of equal rank

   Two pair

         10♦ 10♠ 2♠ 2♣ K♣ ranks higher than 5♣ 5♠ 4♦ 4♥ 10♥

         5♣ 5♠ 4♦ 4♥ 10♥ ranks higher than 5♣ 5♠ 3♣ 3♦ Q♠

         5♣ 5♠ 3♣ 3♦ Q♠ ranks higher than 5♣ 5♠ 3♣ 3♦ J♠

         K♦ K♠ 7♦ 7♥ 8♥ and K♣ K♠ 7♣ 7♥ 8♣, are of equal rank

   One pair

         9♣ 9♦ Q♠ J♥ 5♥ ranks higher than 6♦ 6♥ K♠ 7♥ 4♣

         6♦ 6♥ K♠ 7♥ 4♣ ranks higher than 6♦ 6♥ Q♥ J♠ 2♣

         6♦ 6♥ K♠ 7♥ 4♣, which ranks higher than 6♦ 6♥ Q♥ J♠ 2♣

         6♦ 6♥ Q♥ J♠ 2♣ ranks higher than 6♦ 6♥ Q♠ 8♣ 7♦

         6♦ 6♥ Q♠ 8♣ 7♦ ranks higher than 6♦ 6♥ Q♦ 8♥ 3♠

         8♠ 8♦ 10♥ 6♣ 5♠ and 8♥ 8♣ 10♣ 6♠ 5♣, are of equal rank

   High card

         K♠ 6♣ 5♥ 3♦ 2♣ ranks higher than Q♠ J♦ 6♣ 5♥ 3♣

         Q♠ J♦ 6♣ 5♥ 3♣ ranks higher than Q♠ 10♦ 8♣ 7♦ 4♠

         Q♠ 10♦ 8♣ 7♦ 4♠ ranks higher than Q♥ 10♥ 7♣ 6♥ 4♠

         Q♥ 10♥ 7♣ 6♥ 4♠ ranks higher than Q♣ 10♣ 7♦ 5♣ 4♦

         Q♣ 10♣ 7♦ 5♣ 4♦ ranks higher than Q♥ 10♦ 7♠ 5♠ 2♥

         10♣ 8♠ 7♠ 6♥ 4♦ and 10♦ 8♦ 7♠ 6♣ 4♣, are of equal rank


