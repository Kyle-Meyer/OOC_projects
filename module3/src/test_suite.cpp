/**
 * @file test_suite.cpp
 * @brief Exhaustive test suite for Tic-Tac-Toe game
 * 
 * This file contains comprehensive tests for all game functionality.
 * Tests can be run from main() before starting the actual game.
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#include "board.hpp"
#include <iostream>
#include <cassert>
#include <string>


// Test statistics
static int tests_run = 0;
static int tests_passed = 0;
static int tests_failed = 0;

/**
 * @brief Print test result
 * 
 * @param test_name Name of the test
 * @param passed Whether the test passed
 */
void printTestResult(const std::string& test_name, bool passed) {
    tests_run++;
   if (passed) 
   {
      tests_passed++;
      std::cout << "[PASS] " << test_name << std::endl;
   } 
   else 
   {
      tests_failed++;
      std::cout << "[FAIL] " << test_name << std::endl;
   }
}

/**
 * @brief Test board initialization
 */
void testBoardInitialization() 
{
   std::cout << "\n=== Testing Board Initialization ===" << std::endl;
    
   Board board;
   bool allEmpty = true;
    
   // Check all cells are empty
   for (int i = 0; i < 3; i++) 
   {
        for (int j = 0; j < 3; j++) 
        {
            if (board.getCell(i, j) != ' ') 
            {
                allEmpty = false;
            }
        }
    }
    
    printTestResult("All cells empty after initialization", allEmpty);
    printTestResult("Board not full initially", !board.isFull());
    printTestResult("No winner initially for X", !board.checkWin('X'));
    printTestResult("No winner initially for O", !board.checkWin('O'));
}

/**
 * @brief Test move validation
 */
void testMoveValidation() 
{
   std::cout << "\n=== Testing Move Validation ===" << std::endl;
    
   Board board;
    
    // Test valid moves on empty board
    printTestResult("Valid move at (0,0)", board.isValidMove(0, 0));
    printTestResult("Valid move at (1,1)", board.isValidMove(1, 1));
    printTestResult("Valid move at (2,2)", board.isValidMove(2, 2));
    printTestResult("Valid move at (0,2)", board.isValidMove(0, 2));
    printTestResult("Valid move at (2,0)", board.isValidMove(2, 0));
    
    // Make a move and test occupied cell
    board.makeMove(1, 1, 'X');
    printTestResult("Invalid move on occupied cell (1,1)", !board.isValidMove(1, 1));
    
    // Test out of bounds - negative
    printTestResult("Invalid: row = -1", !board.isValidMove(-1, 0));
    printTestResult("Invalid: col = -1", !board.isValidMove(0, -1));
    printTestResult("Invalid: both = -1", !board.isValidMove(-1, -1));
    
    // Test out of bounds - too large
    printTestResult("Invalid: row = 3", !board.isValidMove(3, 0));
    printTestResult("Invalid: col = 3", !board.isValidMove(0, 3));
    printTestResult("Invalid: both = 3", !board.isValidMove(3, 3));
    printTestResult("Invalid: row = 10", !board.isValidMove(10, 0));
    printTestResult("Invalid: col = 10", !board.isValidMove(0, 10));
}

/**
 * @brief Test making moves
 */
void testMakingMoves() 
{
   std::cout << "\n=== Testing Making Moves ===" << std::endl;
    
   Board board;
    
   // Test successful move
   bool success1 = board.makeMove(0, 0, 'X');
   printTestResult("makeMove returns true for valid move", success1);
   printTestResult("Cell contains X after move", board.getCell(0, 0) == 'X');
    
   // Test overwriting occupied cell
   bool success2 = board.makeMove(0, 0, 'O');
   printTestResult("makeMove returns false for occupied cell", !success2);
   printTestResult("Cell still contains X after failed overwrite", board.getCell(0, 0) == 'X');
    
   // Test another valid move
   board.makeMove(1, 1, 'O');
   printTestResult("Second move successful", board.getCell(1, 1) == 'O');
   
   // Test out of bounds move
   bool success3 = board.makeMove(3, 3, 'X');
   printTestResult("makeMove returns false for out of bounds", !success3);
}

/**
 * @brief Test horizontal (row) wins
 */
void testHorizontalWins() 
{
   std::cout << "\n=== Testing Horizontal Wins ===" << std::endl;
   
   // Test Row 0 - X wins
   Board board1;
   board1.makeMove(0, 0, 'X');
   board1.makeMove(0, 1, 'X');
   board1.makeMove(0, 2, 'X');
   printTestResult("Row 0 win for X", board1.checkWin('X'));
   printTestResult("Row 0 no win for O", !board1.checkWin('O'));
   
   // Test Row 1 - O wins
   Board board2;
   board2.makeMove(1, 0, 'O');
   board2.makeMove(1, 1, 'O');
   board2.makeMove(1, 2, 'O');
   printTestResult("Row 1 win for O", board2.checkWin('O'));
   printTestResult("Row 1 no win for X", !board2.checkWin('X'));
   
   // Test Row 2 - X wins
   Board board3;
   board3.makeMove(2, 0, 'X');
   board3.makeMove(2, 1, 'X');
   board3.makeMove(2, 2, 'X');
   printTestResult("Row 2 win for X", board3.checkWin('X'));
   printTestResult("Row 2 no win for O", !board3.checkWin('O'));
   
   // Test incomplete row - no win
   Board board4;
   board4.makeMove(0, 0, 'X');
   board4.makeMove(0, 1, 'X');
   printTestResult("Incomplete row 0 - no win", !board4.checkWin('X'));
}

/**
 * @brief Test vertical (column) wins
 */
void testVerticalWins() 
{
   std::cout << "\n=== Testing Vertical Wins ===" << std::endl;
   
   // Test Column 0 - O wins
   Board board1;
   board1.makeMove(0, 0, 'O');
   board1.makeMove(1, 0, 'O');
   board1.makeMove(2, 0, 'O');
   printTestResult("Column 0 win for O", board1.checkWin('O'));
   printTestResult("Column 0 no win for X", !board1.checkWin('X'));
   
   // Test Column 1 - X wins
   Board board2;
   board2.makeMove(0, 1, 'X');
   board2.makeMove(1, 1, 'X');
   board2.makeMove(2, 1, 'X');
   printTestResult("Column 1 win for X", board2.checkWin('X'));
   printTestResult("Column 1 no win for O", !board2.checkWin('O'));
   
   // Test Column 2 - O wins
   Board board3;
   board3.makeMove(0, 2, 'O');
   board3.makeMove(1, 2, 'O');
   board3.makeMove(2, 2, 'O');
   printTestResult("Column 2 win for O", board3.checkWin('O'));
   printTestResult("Column 2 no win for X", !board3.checkWin('X'));
   
   // Test incomplete column - no win
   Board board4;
   board4.makeMove(0, 1, 'X');
   board4.makeMove(1, 1, 'X');
   printTestResult("Incomplete column 1 - no win", !board4.checkWin('X'));
}

/**
 * @brief Test diagonal wins
 */
void testDiagonalWins() 
{
   std::cout << "\n=== Testing Diagonal Wins ===" << std::endl;
   
   // Test main diagonal (top-left to bottom-right) - X wins
   Board board1;
   board1.makeMove(0, 0, 'X');
   board1.makeMove(1, 1, 'X');
   board1.makeMove(2, 2, 'X');
   printTestResult("Main diagonal win for X", board1.checkWin('X'));
   printTestResult("Main diagonal no win for O", !board1.checkWin('O'));
   
   // Test anti-diagonal (top-right to bottom-left) - O wins
   Board board2;
   board2.makeMove(0, 2, 'O');
   board2.makeMove(1, 1, 'O');
   board2.makeMove(2, 0, 'O');
   printTestResult("Anti-diagonal win for O", board2.checkWin('O'));
   printTestResult("Anti-diagonal no win for X", !board2.checkWin('X'));
   
   // Test incomplete main diagonal - no win
   Board board3;
   board3.makeMove(0, 0, 'X');
   board3.makeMove(1, 1, 'X');
   printTestResult("Incomplete main diagonal - no win", !board3.checkWin('X'));
   
   // Test incomplete anti-diagonal - no win
   Board board4;
   board4.makeMove(0, 2, 'O');
   board4.makeMove(1, 1, 'O');
   printTestResult("Incomplete anti-diagonal - no win", !board4.checkWin('O'));
}

/**
 * @brief Test mixed symbols (no win conditions)
 */
void testNoWinConditions() 
{
   std::cout << "\n=== Testing No-Win Conditions ===" << std::endl;
   
   // Test diagonal with mixed symbols
   Board board1;
   board1.makeMove(0, 0, 'X');
   board1.makeMove(1, 1, 'O');
   board1.makeMove(2, 2, 'X');
   printTestResult("Mixed diagonal - no win for X", !board1.checkWin('X'));
   printTestResult("Mixed diagonal - no win for O", !board1.checkWin('O'));
   
   // Test row with mixed symbols
   Board board2;
   board2.makeMove(0, 0, 'X');
   board2.makeMove(0, 1, 'O');
   board2.makeMove(0, 2, 'X');
   printTestResult("Mixed row - no win for X", !board2.checkWin('X'));
   printTestResult("Mixed row - no win for O", !board2.checkWin('O'));
   
   // Test column with mixed symbols
   Board board3;
   board3.makeMove(0, 1, 'O');
   board3.makeMove(1, 1, 'X');
   board3.makeMove(2, 1, 'O');
   printTestResult("Mixed column - no win for X", !board3.checkWin('X'));
   printTestResult("Mixed column - no win for O", !board3.checkWin('O'));
}

/**
 * @brief Test tie conditions
 */
void testTieConditions() 
{
   std::cout << "\n=== Testing Tie Conditions ===" << std::endl;
   
   // Create a tie game:
   // X O X
   // X O O
   // O X X
   Board board;
   board.makeMove(0, 0, 'X');
   board.makeMove(0, 1, 'O');
   board.makeMove(0, 2, 'X');
   board.makeMove(1, 0, 'X');
   board.makeMove(1, 1, 'O');
   board.makeMove(1, 2, 'O');
   board.makeMove(2, 0, 'O');
   board.makeMove(2, 1, 'X');
   board.makeMove(2, 2, 'X');
   
   printTestResult("Board is full in tie game", board.isFull());
   printTestResult("No winner for X in tie", !board.checkWin('X'));
   printTestResult("No winner for O in tie", !board.checkWin('O'));
   
   // Test another tie configuration:
   // O X O
   // X X O
   // X O X
   Board board2;
   board2.makeMove(0, 0, 'O');
   board2.makeMove(0, 1, 'X');
   board2.makeMove(0, 2, 'O');
   board2.makeMove(1, 0, 'X');
   board2.makeMove(1, 1, 'X');
   board2.makeMove(1, 2, 'O');
   board2.makeMove(2, 0, 'X');
   board2.makeMove(2, 1, 'O');
   board2.makeMove(2, 2, 'X');
   
   printTestResult("Second tie configuration - board full", board2.isFull());
   printTestResult("Second tie - no winner for X", !board2.checkWin('X'));
   printTestResult("Second tie - no winner for O", !board2.checkWin('O'));
}

/**
 * @brief Test board full detection
 */
void testBoardFull() 
{
   std::cout << "\n=== Testing Board Full Detection ===" << std::endl;
   
   Board board;
   
   printTestResult("Empty board not full", !board.isFull());
   
   // Partially fill
   board.makeMove(0, 0, 'X');
   printTestResult("Board with 1 move not full", !board.isFull());
   
   board.makeMove(1, 1, 'O');
   board.makeMove(2, 2, 'X');
   printTestResult("Board with 3 moves not full", !board.isFull());
   
   // Fill remaining cells
   board.makeMove(0, 1, 'O');
   board.makeMove(0, 2, 'X');
   board.makeMove(1, 0, 'O');
   board.makeMove(1, 2, 'X');
   board.makeMove(2, 0, 'O');
   board.makeMove(2, 1, 'X');
   
   printTestResult("Completely filled board is full", board.isFull());
}

/**
 * @brief Test board reset
 */
void testBoardReset() 
{
   std::cout << "\n=== Testing Board Reset ===" << std::endl;
   
   Board board;
   
   // Fill some cells
   board.makeMove(0, 0, 'X');
   board.makeMove(1, 1, 'O');
   board.makeMove(2, 2, 'X');
   board.makeMove(0, 1, 'O');
   
   // Reset
   board.reset();
   
   bool allEmpty = true;
   for (int i = 0; i < 3; i++) {
       for (int j = 0; j < 3; j++) {
           if (board.getCell(i, j) != ' ') {
               allEmpty = false;
           }
       }
   }
   
   printTestResult("All cells empty after reset", allEmpty);
   printTestResult("Board not full after reset", !board.isFull());
   printTestResult("No winner for X after reset", !board.checkWin('X'));
   printTestResult("No winner for O after reset", !board.checkWin('O'));
}

/**
 * @brief Test all 8 winning lines for X
 */
void testAllWinningLinesX() 
{
   std::cout << "\n=== Testing All Winning Lines for X ===" << std::endl;
   
   // 3 rows
   for (int row = 0; row < 3; row++) 
   {
      Board board;
      for (int col = 0; col < 3; col++) 
      {
         board.makeMove(row, col, 'X');
      }
      printTestResult("X wins row " + std::to_string(row), board.checkWin('X'));
   }
   
   // 3 columns
   for (int col = 0; col < 3; col++) 
   {
      Board board;
      for (int row = 0; row < 3; row++) 
      {
         board.makeMove(row, col, 'X');
      }
      printTestResult("X wins column " + std::to_string(col), board.checkWin('X'));
   }
   
   // Main diagonal
   Board board1;
   board1.makeMove(0, 0, 'X');
   board1.makeMove(1, 1, 'X');
   board1.makeMove(2, 2, 'X');
   printTestResult("X wins main diagonal", board1.checkWin('X'));
   
   // Anti-diagonal
   Board board2;
   board2.makeMove(0, 2, 'X');
   board2.makeMove(1, 1, 'X');
   board2.makeMove(2, 0, 'X');
   printTestResult("X wins anti-diagonal", board2.checkWin('X'));
}

/**
 * @brief Test all 8 winning lines for O
 */
void testAllWinningLinesO() 
{
   std::cout << "\n=== Testing All Winning Lines for O ===" << std::endl;
   
   // 3 rows
   for (int row = 0; row < 3; row++) 
   {
      Board board;
      for (int col = 0; col < 3; col++) 
      {
         board.makeMove(row, col, 'O');
      }
      printTestResult("O wins row " + std::to_string(row), board.checkWin('O'));
   }
   
   // 3 columns
   for (int col = 0; col < 3; col++) 
   {
      Board board;
      for (int row = 0; row < 3; row++) 
      {
         board.makeMove(row, col, 'O');
      }
      printTestResult("O wins column " + std::to_string(col), board.checkWin('O'));
   }
   
   // Main diagonal
   Board board1;
   board1.makeMove(0, 0, 'O');
   board1.makeMove(1, 1, 'O');
   board1.makeMove(2, 2, 'O');
   printTestResult("O wins main diagonal", board1.checkWin('O'));
   
   // Anti-diagonal
   Board board2;
   board2.makeMove(0, 2, 'O');
   board2.makeMove(1, 1, 'O');
   board2.makeMove(2, 0, 'O');
   printTestResult("O wins anti-diagonal", board2.checkWin('O'));
}

/**
 * @brief Test edge cases
 */
void testEdgeCases() 
{
   std::cout << "\n=== Testing Edge Cases ===" << std::endl;
   
   // Test very first cell
   Board board1;
   board1.makeMove(0, 0, 'X');
   printTestResult("Can place in (0,0)", board1.getCell(0, 0) == 'X');
   
   // Test very last cell
   Board board2;
   board2.makeMove(2, 2, 'O');
   printTestResult("Can place in (2,2)", board2.getCell(2, 2) == 'O');
   
   // Test center cell
   Board board3;
   board3.makeMove(1, 1, 'X');
   printTestResult("Can place in center (1,1)", board3.getCell(1, 1) == 'X');
   
   // Test all corners
   Board board4;
   board4.makeMove(0, 0, 'X');
   board4.makeMove(0, 2, 'O');
   board4.makeMove(2, 0, 'X');
   board4.makeMove(2, 2, 'O');
   printTestResult("All corners filled correctly", 
                  board4.getCell(0, 0) == 'X' &&
                  board4.getCell(0, 2) == 'O' &&
                  board4.getCell(2, 0) == 'X' &&
                  board4.getCell(2, 2) == 'O');
}

/**
 * @brief Test complex game scenarios
 */
void testComplexScenarios() 
{
   std::cout << "\n=== Testing Complex Scenarios ===" << std::endl;
   
   // Scenario 1: X wins on last possible move
   Board board1;
   board1.makeMove(0, 0, 'X');
   board1.makeMove(0, 1, 'O');
   board1.makeMove(1, 0, 'X');
   board1.makeMove(1, 1, 'O');
   board1.makeMove(2, 0, 'X');  // X wins column 0
   printTestResult("X wins on move 5", board1.checkWin('X'));
   
   // Scenario 2: Near-tie but X wins
   Board board2;
   board2.makeMove(0, 0, 'X');
   board2.makeMove(0, 1, 'O');
   board2.makeMove(0, 2, 'X');
   board2.makeMove(1, 0, 'O');
   board2.makeMove(1, 1, 'X');
   board2.makeMove(2, 0, 'O');
   board2.makeMove(2, 2, 'X');  // X wins main diagonal
   printTestResult("X wins diagonal with board nearly full", board2.checkWin('X'));
   printTestResult("Board not completely full", !board2.isFull());
   
   // Scenario 3: Blocking scenario (no win yet)
   Board board3;
   board3.makeMove(0, 0, 'X');
   board3.makeMove(0, 1, 'X');
   board3.makeMove(1, 1, 'O');  // O blocks potential X win
   printTestResult("After block - no winner yet", !board3.checkWin('X') && !board3.checkWin('O'));
}

/**
 * @brief Run all tests
 * 
 * @return 0 if all tests passed, 1 if any failed
 */
int runAllTests() 
{
   std::cout << "\n";
   std::cout << "========================================\n";
   std::cout << "  TIC-TAC-TOE EXHAUSTIVE TEST SUITE\n";
   std::cout << "========================================\n";
   
   tests_run = 0;
   tests_passed = 0;
   tests_failed = 0;
   
   // Run all test categories
   testBoardInitialization();
   testMoveValidation();
   testMakingMoves();
   testHorizontalWins();
   testVerticalWins();
   testDiagonalWins();
   testNoWinConditions();
   testTieConditions();
   testBoardFull();
   testBoardReset();
   testAllWinningLinesX();
   testAllWinningLinesO();
   testEdgeCases();
   testComplexScenarios();
   
   // Print summary
   std::cout << "\n========================================\n";
   std::cout << "  TEST SUMMARY\n";
   std::cout << "========================================\n";
   std::cout << "Total tests run:    " << tests_run << std::endl;
   std::cout << "Tests passed:       " << tests_passed << " (" 
        << (tests_run > 0 ? (tests_passed * 100 / tests_run) : 0) << "%)" << std::endl;
   std::cout << "Tests failed:       " << tests_failed << std::endl;
   std::cout << "========================================\n\n";
   
   if (tests_failed == 0) 
   {
      std::cout << "ALL TESTS PASSED!\n\n";
      return 0;
   } 
   else 
   {
      std::cout << "SOME TESTS FAILED!\n\n";
      return 1;
   }
}
