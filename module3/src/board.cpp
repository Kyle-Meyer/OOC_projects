/**
 * @file Board.cpp
 * @brief Implementation of Board class methods
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#include "board.hpp"
#include <iostream>

Board::Board(){
   reset();
}

bool Board::isValidMove(int row, int col) const 
{
    // Check bounds
    if (row < 0 || row > 2 || col < 0 || col > 2) 
        return false;
    
    // Check if cell is empty
    return grid[row][col] == ' ';
}

bool Board::makeMove(int row, int col, char player) 
{
    if (!isValidMove(row, col))
        return false;
    
    grid[row][col] = player;
    return true;
}

char Board::getCell(int row, int col) const 
{
    return grid[row][col];
}

bool Board::checkWin(char player) const 
{
    // Check rows
    for (int i = 0; i < 3; i++) 
        if (grid[i][0] == player && grid[i][1] == player && grid[i][2] == player) 
            return true;
    
    // Check columns
    for (int j = 0; j < 3; j++)
        if (grid[0][j] == player && grid[1][j] == player && grid[2][j] == player) 
            return true;
    
    // Check diagonals
    if (grid[0][0] == player && grid[1][1] == player && grid[2][2] == player) 
        return true;

    if (grid[0][2] == player && grid[1][1] == player && grid[2][0] == player) 
        return true;
   
    return false;
}

bool Board::isFull() const 
{  
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) 
            if (grid[i][j] == ' ') 
                return false;
    return true;
}

void Board::reset() 
{
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++) 
            grid[i][j] = ' ';
}

void Board::display() const 
{
   std::cout << "\n";
   std::cout << "     1   2   3\n";
   std::cout << "   +---+---+---+\n";
    
   for (int i = 0; i < 3; i++) 
   {
      std::cout << " " << (i + 1) << " |";
         for (int j = 0; j < 3; j++)
            std::cout << " " << grid[i][j] << " |";
      std::cout << "\n";
      std::cout << "   +---+---+---+\n";
   }
   std::cout << "\n";
}
