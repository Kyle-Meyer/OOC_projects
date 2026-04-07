#include "game.hpp"
#include "human_player.hpp"
#include "computer_player.hpp"
#include "gui_app.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <limits>
#include <cctype>

static int promptInt(const std::string& msg, int lo, int hi)
{
   int val = 0;
   while (true)
   {
      std::cout << msg;
      if (std::cin >> val && val >= lo && val <= hi)
      {
         std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
         return val;
      }
      std::cin.clear();
      std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
      std::cout << "  Please enter a number between " << lo << " and " << hi << ".\n";
   }
}

int main(int argc, char* argv[])
{
   // Pass --gui (or -g) to launch the Dear ImGui interface
   for (int i = 1; i < argc; ++i) {
      if (std::string(argv[i]) == "--gui" || std::string(argv[i]) == "-g") {
         runGui();
         return 0;
      }
   }

   // ---- original CLI mode below ----
   {
   std::cout << "=============================\n";
   std::cout << "   5-Card Draw Poker\n";
   std::cout << "=============================\n\n";

   int numPlayers    = promptInt("Number of players (2-7): ", 2, 7);
   int startingChips = promptInt("Starting chips per player: ", 10, 100000);
   int ante          = promptInt("Ante per round: ", 1, startingChips / 2);

   std::vector<std::unique_ptr<Player>> players;

   for (int i = 0; i < numPlayers; ++i)
   {
      std::cout << "\nPlayer " << (i + 1) << " name: ";
      std::string name;
      std::getline(std::cin, name);
      if (name.empty()) name = "Player" + std::to_string(i + 1);

      std::cout << "Type — (h)uman or (c)omputer [default h]: ";
      std::string type;
      std::getline(std::cin, type);

      if (!type.empty() &&
          std::tolower(static_cast<unsigned char>(type[0])) == 'c')
         players.push_back(std::make_unique<ComputerPlayer>(name, startingChips));
      else
         players.push_back(std::make_unique<HumanPlayer>(name, startingChips));
   }

   std::cout << "\n";
   Game game(std::move(players), ante);
   game.run();

   return 0;
   } // end CLI block
}
