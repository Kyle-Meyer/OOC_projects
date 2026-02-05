/**
 * @file ComputerPlayer.cpp
 * @brief Implementation of computer AI player
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#include "computer_player.hpp"
#include <cstdlib>
#include <vector>

using namespace std;

/**
 * @brief Helper function to find a winning move
 * 
 * @param board The current board state
 * @param player The player to check winning moves for
 * @param row Output parameter for winning row (-1 if none found)
 * @param col Output parameter for winning column (-1 if none found)
 * @return true if a winning move was found
 */
static bool findWinningMove(Board& board, char player, int& row, int& col) 
{
    // Try each empty cell
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (board.isValidMove(i, j)) 
            {
                // Try this move
                board.makeMove(i, j, player);
                
                // Check if it wins
                if (board.checkWin(player)) 
                {
                    // Undo the move
                    board.makeMove(i, j, ' ');
                    row = i;
                    col = j;
                    return true;
                }
                
                // Undo the move
                board.makeMove(i, j, ' ');
            }
        }
    }
    
    row = -1;
    col = -1;
    return false;
}

/**
 * @brief Helper function to find a random valid move
 * 
 * @param board The current board state
 * @param row Output parameter for random row
 * @param col Output parameter for random column
 */
static void findRandomMove(const Board& board, int& row, int& col) 
{
   vector<pair<int, int>> availableMoves;
    
   // Collect all available moves
   for (int i = 0; i < 3; i++) 
      for (int j = 0; j < 3; j++) 
         if (board.isValidMove(i, j)) 
            availableMoves.push_back(make_pair(i, j));
    
   // Pick random move
   if (!availableMoves.empty()) 
   {
      int randomIndex = rand() % availableMoves.size();
      row = availableMoves[randomIndex].first;
      col = availableMoves[randomIndex].second;
   }
}

void getComputerMove(Board& board, int& row, int& col) {
    // Priority 1: Try to win
   if (findWinningMove(board, 'O', row, col))
      return;
    
   // Priority 2: Block opponent's win
   if (findWinningMove(board, 'X', row, col))
      return;
    
   // Priority 3: Random move
   findRandomMove(board, row, col);
}
