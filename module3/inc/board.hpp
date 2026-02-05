/**
 * @file Board.h
 * @brief Board class declaration for Tic-Tac-Toe game
 * 
 * The Board class manages the game state, including the 3x3 grid of cells,
 * move validation, and win/tie detection.
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#ifndef BOARD_H
#define BOARD_H

/**
 * @class Board
 * @brief Manages the Tic-Tac-Toe game board state and logic
 * 
 * The board is represented as a 3x3 grid with characters:
 * - ' ' for empty cells
 * - 'X' for human player
 * - 'O' for computer player
 */
class Board {
public:
    /**
     * @brief Default constructor - initializes an empty board
     */
    Board();
    
    /**
     * @brief Checks if a move is valid
     * 
     * @param row Row index (0-2)
     * @param col Column index (0-2)
     * @return true if the move is valid, false otherwise
     */
    bool isValidMove(int row, int col) const;
    
    /**
     * @brief Makes a move on the board
     * 
     * @param row Row index (0-2)
     * @param col Column index (0-2)
     * @param player The player symbol ('X' or 'O')
     * @return true if move was successful, false if move was invalid
     */
    bool makeMove(int row, int col, char player);
    
    /**
     * @brief Gets the symbol at a specific cell
     * 
     * @param row Row index (0-2)
     * @param col Column index (0-2)
     * @return The character at that position
     */
    char getCell(int row, int col) const;
    
    /**
     * @brief Checks if a player has won
     * 
     * @param player The player symbol to check ('X' or 'O')
     * @return true if the player has won
     */
    bool checkWin(char player) const;
    
    /**
     * @brief Checks if the board is completely filled
     * 
     * @return true if all cells are occupied
     */
    bool isFull() const;
    
    /**
     * @brief Resets the board to initial empty state
     */
    void reset();
    
    /**
     * @brief Displays the board to console
     */
    void display() const;

private:
    char grid[3][3];  ///< 3x3 grid storing the board state
};

#endif // BOARD_H
