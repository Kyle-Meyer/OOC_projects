/**
 * @file main.cpp
 * @brief Main game loop for Tic-Tac-Toe
 * 
 * Implements the main game flow, coordinating the board, human player,
 * and computer player.
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#include "board.hpp"
#include "human_player.hpp"
#include "computer_player.hpp"
#include "test_suite.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>


/**
 * @brief Main game function
 * 
 * Runs the complete game from start to finish
 */
void playGame() 
{
   Board board;
   
   // Display welcome message
   std::cout << "========================================\n";
   std::cout << "        TIC-TAC-TOE GAME\n";
   std::cout << "========================================\n";
   std::cout << "You are X, Computer is O\n";
   std::cout << "You play first!\n";
   std::cout << "Enter moves as: row col (e.g., '1 3')\n";
   std::cout << "========================================\n\n";
   
   bool gameOver = false;
   bool humanTurn = true;  // Human (X) goes first
   
   while (!gameOver) 
   {
      // Display current board
      board.display();
      std::cout << "----------------------------------------\n";
       
      int row, col;
       
      if (humanTurn) 
      {
         // Human's turn
         std::cout << "Your turn (X):\n";
         getHumanMove(board, row, col);
         board.makeMove(row, col, 'X');
           
         // Check if human won
         if (board.checkWin('X')) 
         {
            board.display();
            std::cout << "========================================\n";
            std::cout << "    CONGRATULATIONS! YOU WIN!\n";
            std::cout << "========================================\n\n";
            gameOver = true;
         }
      } 
      else
      {
         // Computer's turn
         std::cout << "Computer's turn (O)...\n";
         getComputerMove(board, row, col);
         board.makeMove(row, col, 'O');
         std::cout << "Computer places O at row " << (row + 1) 
                << ", col " << (col + 1) << "\n\n";
           
         // Check if computer won
         if (board.checkWin('O')) 
         {
            board.display();
            std::cout << "========================================\n";
            std::cout << "       COMPUTER WINS!\n";
            std::cout << "========================================\n\n";
            gameOver = true;
         }
      }
       
      // Check for tie
      if (!gameOver && board.isFull()) 
      {
         board.display();
         std::cout << "========================================\n";
         std::cout << "          IT'S A TIE!\n";
         std::cout << "========================================\n\n";
         gameOver = true;
      }
       
      // Switch turns
      humanTurn = !humanTurn;
   }
}

/**
 * @brief Main entry point
 * 
 * @return 0 on success
 */
int main(int argc, char* argv[]) {
   // Check if user wants to run tests
   if (argc > 1 && std::string(argv[1]) == "--test") 
   {
     return runAllTests();
   }
   
   // Ask user if they want to run tests first
   std::cout << "Run tests before playing? (y/n): ";
   char choice;
   std::cin >> choice;
   std::cin.ignore(10000, '\n');
   
   if (choice == 'y' || choice == 'Y') 
   {
      int testResult = runAllTests();
       
      if (testResult != 0) 
      {
         std::cout << "Tests failed! Fix issues before playing.\n";
         return 1;
      }
       
      std::cout << "Press Enter to start game...";
      std::cin.get();
   }
   
   // Play the game
   playGame();
   
   return 0;
}
