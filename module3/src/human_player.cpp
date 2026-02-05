/**
 * @file HumanPlayer.cpp
 * @brief Implementation of human player input functions
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#include "human_player.hpp"
#include <iostream>


void getHumanMove(const Board& board, int& row, int& col) 
{
   while (true) 
   {
      std::cout << "Enter your move (row col), where row and col are 1-3: ";
        
      int inputRow, inputCol;
        
      // Try to read two integers
      if (!(std::cin >> inputRow >> inputCol)) 
      {
         std::cout << "Invalid input. Please enter two numbers.\n";
         std::cin.clear();
         std::cin.ignore(10000, '\n');
         continue;
      }
        
      // Convert to 0-indexed
      row = inputRow - 1;
      col = inputCol - 1;
        
      // Validate range
      if (row < 0 || row > 2 || col < 0 || col > 2) 
      {
         std::cout << "Invalid input. Row and column must be between 1 and 3.\n";
         continue;
      }
        
      // Validate move is legal
      if (!board.isValidMove(row, col)) 
      {
         std::cout << "That cell is already occupied. Try again.\n";
         continue;
      }
        
      // Valid move found
      break;
    }
}
