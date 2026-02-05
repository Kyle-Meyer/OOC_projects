/**
 * @file HumanPlayer.h
 * @brief Human player functions for console input
 * 
 * Handles getting and validating moves from the human player.
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#ifndef HUMANPLAYER_H
#define HUMANPLAYER_H

#include "board.hpp"

/**
 * @brief Gets a move from the human player via console input
 * 
 * Prompts the user to enter row and column (1-indexed), validates
 * the input, and converts to 0-indexed coordinates.
 * 
 * @param board The current board state (for validation)
 * @param row Output parameter for the chosen row (0-2)
 * @param col Output parameter for the chosen column (0-2)
 */
void getHumanMove(const Board& board, int& row, int& col);

#endif // HUMANPLAYER_H
