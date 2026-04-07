#pragma once
#include "card.hpp"
#include <string>
#include <vector>
#include <mutex>
#include <condition_variable>

enum class GuiAction { None, Bet, Discard };

struct GuiState
{
    std::mutex              mtx;
    std::condition_variable cv;

    // Game log (captured from std::cout via LogBuf)
    std::vector<std::string> log;

    // Pending action for the human GUI player
    GuiAction         action   = GuiAction::None;
    int               toCall   = 0;
    int               pot      = 0;
    std::vector<Card> hand;
    std::vector<bool> discardSel;   // size 5 during Discard phase

    // Set by the render thread to unblock GuiPlayer
    bool actionReady = false;
    int  betResult   = 0;           // -1 = fold, >= 0 = chips put in

    bool gameOver = false;
};
