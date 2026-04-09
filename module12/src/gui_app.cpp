#include "gui_state.hpp"
#include "gui_player.hpp"
#include "computer_player.hpp"
#include "game.hpp"
#include "card.hpp"
#include "card_textures.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <sstream>
#include <thread>
#include <memory>
#include <string>
#include <vector>
#include <algorithm>
#include <cstring>

// ---------------------------------------------------------------------------
// Captures std::cout lines into GuiState::log
// ---------------------------------------------------------------------------
class LogBuf : public std::streambuf
{
    GuiState&   state_;
    std::string line_;
protected:
    int overflow(int c) override
    {
        if (c == EOF) return EOF;
        if (c == '\n') {
            std::lock_guard<std::mutex> lk(state_.mtx);
            state_.log.push_back(line_);
            line_.clear();
        } else {
            line_ += static_cast<char>(c);
        }
        return c;
    }
public:
    explicit LogBuf(GuiState& s) : state_(s) {}
    ~LogBuf()
    {
        if (!line_.empty()) {
            std::lock_guard<std::mutex> lk(state_.mtx);
            state_.log.push_back(line_);
        }
    }
};

// ---------------------------------------------------------------------------
// Card rendering helpers
// ---------------------------------------------------------------------------
static std::string cardLabel(const Card& c)
{
    std::string s;
    if      (c.rank == 14) s = "A";
    else if (c.rank == 13) s = "K";
    else if (c.rank == 12) s = "Q";
    else if (c.rank == 11) s = "J";
    else if (c.rank == 10) s = "10";
    else                   s = std::to_string(c.rank);

    switch (c.suit) {
        case 'H': s += " \xE2\x99\xA5"; break;   // ♥ UTF-8
        case 'D': s += " \xE2\x99\xA6"; break;   // ♦
        case 'C': s += " \xE2\x99\xA3"; break;   // ♣
        case 'S': s += " \xE2\x99\xA0"; break;   // ♠
        default:  s += c.suit;
    }
    return s;
}

static bool isRedSuit(char suit) { return suit == 'H' || suit == 'D'; }

// Draw a card button; uses a PNG texture when available, falls back to a
// coloured text button otherwise.  Returns true if clicked.
static bool cardButton(const char* id, const Card& c, bool selected,
                       GLuint tex = 0, ImVec2 sz = {71, 96})
{
    bool clicked = false;

    if (tex) {
        // Tint the image orange-red when selected for discard
        ImVec4 tint = selected ? ImVec4(1.f, 0.55f, 0.45f, 1.f) : ImVec4(1,1,1,1);
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
        clicked = ImGui::ImageButton(id, (ImTextureID)(intptr_t)tex, sz,
                                     {0,0}, {1,1}, {0,0,0,0}, tint);
        ImGui::PopStyleVar();

        // Draw a coloured border when selected
        if (selected) {
            ImVec2 pMin = ImGui::GetItemRectMin();
            ImVec2 pMax = ImGui::GetItemRectMax();
            ImGui::GetWindowDrawList()->AddRect(pMin, pMax,
                                               IM_COL32(220, 60, 30, 255),
                                               0.f, 0, 3.f);
        }
    } else {
        // Fallback: coloured text button
        ImVec4 col;
        if (selected)
            col = {0.85f, 0.25f, 0.10f, 1.0f};
        else if (isRedSuit(c.suit))
            col = {0.75f, 0.08f, 0.08f, 0.90f};
        else
            col = {0.18f, 0.18f, 0.22f, 0.90f};

        ImGui::PushID(id);
        ImGui::PushStyleColor(ImGuiCol_Button,        col);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, {col.x+0.15f, col.y+0.15f, col.z+0.15f, 1.f});
        ImGui::PushStyleColor(ImGuiCol_ButtonActive,  {col.x-0.10f, col.y-0.10f, col.z-0.10f, 1.f});
        clicked = ImGui::Button(cardLabel(c).c_str(), sz);
        ImGui::PopStyleColor(3);
        ImGui::PopID();
    }

    return clicked;
}

// ---------------------------------------------------------------------------
// Main GUI entry point
// ---------------------------------------------------------------------------
void runGui()
{
    if (!glfwInit()) {
        std::cerr << "GLFW init failed\n";
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    GLFWwindow* window = glfwCreateWindow(960, 680, "5-Card Draw Poker", nullptr, nullptr);
    if (!window) { glfwTerminate(); return; }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.IniFilename = nullptr;   // don't save layout to disk
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // -----------------------------------------------------------------------
    // Load card textures (must be done after the OpenGL context is current)
    // -----------------------------------------------------------------------
    CardTextures textures;
    if (!textures.load("cards/"))
        std::cerr << "[runGui] Some card textures failed to load.\n";

    // -----------------------------------------------------------------------
    // Setup screen state
    // -----------------------------------------------------------------------
    enum class Screen { Setup, Game };
    Screen screen = Screen::Setup;

    static int  numPlayers    = 2;
    static int  startingChips = 100;
    static int  ante          = 5;
    static char names[7][64]  = {
        "You", "CPU-1", "CPU-2", "CPU-3", "CPU-4", "CPU-5", "CPU-6"
    };
    static bool human[7] = { true, false, false, false, false, false, false };

    GuiState       state;
    std::thread    gameThread;
    bool           threadLaunched = false;

    // -----------------------------------------------------------------------
    // Render loop
    // -----------------------------------------------------------------------
    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int winW, winH;
        glfwGetWindowSize(window, &winW, &winH);

        // ===================================================================
        // SETUP SCREEN
        // ===================================================================
        if (screen == Screen::Setup)
        {
            ImGui::SetNextWindowPos({(float)winW * 0.5f - 220, (float)winH * 0.5f - 200},
                                    ImGuiCond_Always, {0.5f, 0.5f});
            ImGui::SetNextWindowSize({440, 0});
            ImGui::Begin("5-Card Draw Poker: Setup",
                         nullptr,
                         ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoMove   |
                         ImGuiWindowFlags_NoCollapse);

            ImGui::SliderInt("Players", &numPlayers, 2, 7);
            ImGui::InputInt("Starting chips", &startingChips);
            if (startingChips < 10) startingChips = 10;
            ImGui::InputInt("Ante", &ante);
            if (ante < 1) ante = 1;
            if (ante > startingChips / 2) ante = startingChips / 2;

            ImGui::Separator();
            ImGui::Text("Player configuration:");

            for (int i = 0; i < numPlayers; ++i)
            {
                ImGui::PushID(i);
                char label[16];
                std::snprintf(label, sizeof(label), "P%d name", i + 1);
                ImGui::InputText(label, names[i], sizeof(names[i]));
                ImGui::SameLine();
                ImGui::Checkbox("Human", &human[i]);
                ImGui::PopID();
            }

            ImGui::Separator();
            if (ImGui::Button("Start Game", {120, 36}))
                screen = Screen::Game;

            ImGui::End();
        }

        // ===================================================================
        // GAME SCREEN
        // ===================================================================
        else
        {
            // Launch game thread on first frame of Game screen
            if (!threadLaunched)
            {
                threadLaunched = true;

                // Capture setup values by copy for the thread lambda
                int  np    = numPlayers;
                int  chips = startingChips;
                int  a     = ante;
                std::vector<std::string> pnames(np);
                std::vector<bool>        phuman(np);
                for (int i = 0; i < np; ++i) {
                    pnames[i] = names[i];
                    phuman[i] = human[i];
                }

                gameThread = std::thread([&state, np, chips, a,
                                          pnames, phuman]()
                {
                    LogBuf logBuf(state);
                    std::streambuf* old = std::cout.rdbuf(&logBuf);

                    std::vector<std::unique_ptr<Player>> players;
                    for (int i = 0; i < np; ++i)
                    {
                        if (phuman[i])
                            players.push_back(
                                std::make_unique<GuiPlayer>(pnames[i], chips, state));
                        else
                            players.push_back(
                                std::make_unique<ComputerPlayer>(pnames[i], chips));
                    }

                    Game game(std::move(players), a);
                    game.run();

                    std::cout.rdbuf(old);

                    std::lock_guard<std::mutex> lk(state.mtx);
                    state.gameOver = true;
                    // Wake any waiting GuiPlayer (e.g. if game ends mid-wait)
                    state.betResult   = -1;
                    state.actionReady = true;
                    state.cv.notify_all();
                });
            }

            // ---------------------------------------------------------------
            // Left panel: game log
            // ---------------------------------------------------------------
            float logW = winW * 0.55f;
            ImGui::SetNextWindowPos({0, 0});
            ImGui::SetNextWindowSize({logW, (float)winH});
            ImGui::Begin("Game Log", nullptr,
                         ImGuiWindowFlags_NoResize  |
                         ImGuiWindowFlags_NoMove    |
                         ImGuiWindowFlags_NoCollapse|
                         ImGuiWindowFlags_HorizontalScrollbar);
            {
                std::lock_guard<std::mutex> lk(state.mtx);
                for (auto& line : state.log)
                    ImGui::TextUnformatted(line.c_str());
                // Auto-scroll to bottom
                if (ImGui::GetScrollY() >= ImGui::GetScrollMaxY() - 4.0f)
                    ImGui::SetScrollHereY(1.0f);
            }
            ImGui::End();

            // ---------------------------------------------------------------
            // Right panel: action or status
            // ---------------------------------------------------------------
            float actionX = logW;
            float actionW = winW - logW;

            ImGui::SetNextWindowPos({actionX, 0});
            ImGui::SetNextWindowSize({actionW, (float)winH});

            // Snapshot shared state under lock
            GuiAction         curAction;
            int               curToCall, curPot;
            std::vector<Card> curHand;
            std::vector<bool> curDiscardSel;
            bool              curGameOver;
            {
                std::lock_guard<std::mutex> lk(state.mtx);
                curAction     = state.action;
                curToCall     = state.toCall;
                curPot        = state.pot;
                curHand       = state.hand;
                curDiscardSel = state.discardSel;
                curGameOver   = state.gameOver;
            }

            // -- BET panel --
            if (curAction == GuiAction::Bet)
            {
                ImGui::Begin("Your Turn, Bet", nullptr,
                             ImGuiWindowFlags_NoResize  |
                             ImGuiWindowFlags_NoMove    |
                             ImGuiWindowFlags_NoCollapse);

                ImGui::Text("Pot: %d", curPot);
                if (curToCall > 0)
                    ImGui::Text("To call: %d", curToCall);
                else
                    ImGui::Text("You may check for free.");
                ImGui::Separator();

                ImGui::Text("Your hand:");
                ImGui::Spacing();
                for (int i = 0; i < static_cast<int>(curHand.size()); ++i) {
                    if (i > 0) ImGui::SameLine(0, 6);
                    char id[8]; std::snprintf(id, sizeof(id), "hc%d", i);
                    cardButton(id, curHand[i], false, textures.get(curHand[i]));
                }

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                // Fold
                ImGui::PushStyleColor(ImGuiCol_Button, {0.65f, 0.10f, 0.10f, 1.0f});
                if (ImGui::Button("Fold", {90, 38})) {
                    std::lock_guard<std::mutex> lk(state.mtx);
                    state.betResult   = -1;
                    state.actionReady = true;
                    state.cv.notify_one();
                }
                ImGui::PopStyleColor();

                ImGui::SameLine(0, 10);

                // Call / Check
                ImGui::PushStyleColor(ImGuiCol_Button, {0.12f, 0.50f, 0.12f, 1.0f});
                const char* callLbl = curToCall == 0 ? "Check" : "Call";
                if (ImGui::Button(callLbl, {90, 38})) {
                    std::lock_guard<std::mutex> lk(state.mtx);
                    state.betResult   = curToCall;
                    state.actionReady = true;
                    state.cv.notify_one();
                }
                ImGui::PopStyleColor();

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Text("Raise:");

                static int raiseAmt = 10;
                ImGui::SetNextItemWidth(100);
                ImGui::InputInt("chips##raise", &raiseAmt);
                if (raiseAmt < 1) raiseAmt = 1;

                ImGui::SameLine(0, 8);
                ImGui::PushStyleColor(ImGuiCol_Button, {0.20f, 0.30f, 0.70f, 1.0f});
                if (ImGui::Button("Raise", {80, 32})) {
                    std::lock_guard<std::mutex> lk(state.mtx);
                    state.betResult   = curToCall + raiseAmt;
                    state.actionReady = true;
                    state.cv.notify_one();
                }
                ImGui::PopStyleColor();

                ImGui::End();
            }

            // -- DISCARD panel --
            else if (curAction == GuiAction::Discard)
            {
                ImGui::Begin("Your Turn, Discard", nullptr,
                             ImGuiWindowFlags_NoResize  |
                             ImGuiWindowFlags_NoMove    |
                             ImGuiWindowFlags_NoCollapse);

                ImGui::TextWrapped("Click a card to toggle it for discard (highlighted = discard). "
                                   "Then press Confirm.");
                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                for (int i = 0; i < static_cast<int>(curHand.size()); ++i) {
                    if (i > 0) ImGui::SameLine(0, 6);
                    bool sel = (i < static_cast<int>(curDiscardSel.size())) && curDiscardSel[i];
                    char id[8]; std::snprintf(id, sizeof(id), "dc%d", i);
                    if (cardButton(id, curHand[i], sel, textures.get(curHand[i]))) {
                        // Toggle in shared state
                        std::lock_guard<std::mutex> lk(state.mtx);
                        if (i < static_cast<int>(state.discardSel.size()))
                            state.discardSel[i] = !state.discardSel[i];
                    }
                }

                ImGui::Spacing();
                // Count selected
                int selCount = 0;
                for (bool b : curDiscardSel) if (b) ++selCount;
                ImGui::Text("Discarding: %d card(s)", selCount);

                ImGui::Spacing();
                ImGui::Separator();
                ImGui::Spacing();

                ImGui::PushStyleColor(ImGuiCol_Button, {0.15f, 0.50f, 0.20f, 1.0f});
                if (ImGui::Button("Confirm Discards", {160, 38})) {
                    std::lock_guard<std::mutex> lk(state.mtx);
                    state.actionReady = true;
                    state.cv.notify_one();
                }
                ImGui::PopStyleColor();

                ImGui::End();
            }

            // -- WAITING / GAME OVER panel --
            else
            {
                ImGui::Begin("Status", nullptr,
                             ImGuiWindowFlags_NoResize  |
                             ImGuiWindowFlags_NoMove    |
                             ImGuiWindowFlags_NoCollapse);

                if (curGameOver) {
                    ImGui::TextColored({0.3f, 1.0f, 0.3f, 1.0f}, "Game over!");
                    ImGui::Spacing();
                    ImGui::TextWrapped("See the log on the left for the final results.");
                } else {
                    ImGui::Text("Waiting...");
                    ImGui::TextWrapped("(Computer players are acting or cards are being dealt.)");
                }
                ImGui::End();
            }
        }

        // ---------------------------------------------------------------
        // Render
        // ---------------------------------------------------------------
        ImGui::Render();
        int fbW, fbH;
        glfwGetFramebufferSize(window, &fbW, &fbH);
        glViewport(0, 0, fbW, fbH);
        glClearColor(0.08f, 0.08f, 0.10f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);
    }

    // User closed the window — unblock any waiting GuiPlayer
    {
        std::lock_guard<std::mutex> lk(state.mtx);
        state.betResult   = -1;
        state.actionReady = true;
        state.gameOver    = true;
    }
    state.cv.notify_all();

    if (threadLaunched && gameThread.joinable())
        gameThread.join();

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
