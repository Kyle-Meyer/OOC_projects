#include "gui_player.hpp"
#include <algorithm>

GuiPlayer::GuiPlayer(const std::string& name, int chips, GuiState& state)
    : Player(name, chips), state_(state) {}

int GuiPlayer::placeBet(int toCall, int pot)
{
    // Tell the render thread we need a bet decision
    {
        std::lock_guard<std::mutex> lk(state_.mtx);
        if (state_.gameOver) return -1;   // window closed — fold immediately
        state_.action      = GuiAction::Bet;
        state_.toCall      = toCall;
        state_.pot         = pot;
        state_.hand        = cards_;
        state_.actionReady = false;
    }

    // Block until the render thread sets actionReady (or the window closes)
    std::unique_lock<std::mutex> lk(state_.mtx);
    state_.cv.wait(lk, [this]{ return state_.actionReady || state_.gameOver; });
    if (state_.gameOver) return -1;
    state_.action      = GuiAction::None;
    state_.actionReady = false;
    return state_.betResult;
}

std::vector<int> GuiPlayer::chooseDiscards()
{
    {
        std::lock_guard<std::mutex> lk(state_.mtx);
        if (state_.gameOver) return {};   // window closed — discard nothing
        state_.action      = GuiAction::Discard;
        state_.hand        = cards_;
        state_.discardSel.assign(5, false);
        state_.actionReady = false;
    }

    std::unique_lock<std::mutex> lk(state_.mtx);
    state_.cv.wait(lk, [this]{ return state_.actionReady || state_.gameOver; });
    if (state_.gameOver) return {};
    state_.action      = GuiAction::None;
    state_.actionReady = false;

    std::vector<int> indices;
    for (int i = 0; i < static_cast<int>(state_.discardSel.size()); ++i)
        if (state_.discardSel[i]) indices.push_back(i);
    return indices;
}
