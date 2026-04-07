#pragma once
#include "card.hpp"
#include <string>
#include <unordered_map>

// Loads one PNG per card from a directory and exposes them as OpenGL texture IDs.
//
// Expected filenames:  {rank}{suit}.png   e.g.  AS.png  KH.png  10C.png  2D.png
//   Rank tokens : A  2 3 4 5 6 7 8 9 10  J  Q  K
//   Suit tokens : C  D  H  S
//   Card back   : back.png
//
// All 52 card PNGs + back.png must live in the same directory.
// Call load() once after the OpenGL context is created.

typedef unsigned int GLuint;   // matches GL type without pulling in full GL header

class CardTextures
{
public:
    ~CardTextures() { unload(); }

    // Load every card PNG from `dir`.  Returns true if all 53 files loaded OK.
    // Logs a warning to stderr for each missing or unreadable file.
    bool load(const std::string& dir);

    void unload();

    // OpenGL texture ID for a card; returns 0 if not loaded.
    GLuint get(const Card& c) const;

    // OpenGL texture ID for the card back.
    GLuint back() const { return back_; }

    // Pixel dimensions of the loaded card images (taken from the first card loaded).
    float cardW() const { return w_; }
    float cardH() const { return h_; }

    // Returns the expected filename for a card, e.g. "AS.png" or "10H.png".
    static std::string filename(const Card& c);

private:
    GLuint loadOne(const std::string& path);

    std::unordered_map<std::string, GLuint> tex_;
    GLuint back_ = 0;
    float  w_    = 71.f;
    float  h_    = 96.f;
};
