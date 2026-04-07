#pragma once
#include "player.hpp"
#include "gui_state.hpp"

// A human player whose placeBet / chooseDiscards block until
// the ImGui render thread provides a response via GuiState.
class GuiPlayer : public Player
{
public:
    GuiPlayer(const std::string& name, int chips, GuiState& state);

    int              placeBet(int toCall, int pot) override;
    std::vector<int> chooseDiscards()              override;

private:
    GuiState& state_;
};
