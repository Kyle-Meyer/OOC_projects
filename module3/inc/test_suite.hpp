/**
 * @file test_suite.hpp
 * @brief Header for test suite
 * 
 * @author Kyle
 * @date 2026-02-01
 */

#ifndef TEST_SUITE_HPP
#define TEST_SUITE_HPP

/**
 * @brief Run all tests for the Tic-Tac-Toe game
 * 
 * Runs an exhaustive test suite covering:
 * - Board initialization
 * - Move validation
 * - Making moves
 * - All 8 winning lines for both players
 * - Tie conditions
 * - Board reset
 * - Edge cases
 * - Complex game scenarios
 * 
 * @return 0 if all tests passed, 1 if any tests failed
 */
int runAllTests();

#endif // TEST_SUITE_HPP
