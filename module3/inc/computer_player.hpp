/**
 * @file ComputerPlayer.h
 * @brief Computer AI player functions
 * 
 * Implements strategic move selection for the computer opponent.
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#ifndef COMPUTERPLAYER_H
#define COMPUTERPLAYER_H

#include "board.hpp"

/**
 * @brief Gets a move from the computer player using AI strategy
 * 
 * Strategy (in priority order):
 * 1. Take a winning move if available
 * 2. Block the opponent's winning move if necessary
 * 3. Take a random available move
 * 
 * @param board The current board state
 * @param row Output parameter for the chosen row (0-2)
 * @param col Output parameter for the chosen column (0-2)
 */
void getComputerMove(Board& board, int& row, int& col);

#endif // COMPUTERPLAYER_H
