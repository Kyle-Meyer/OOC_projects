// STB_IMAGE_IMPLEMENTATION must be defined in exactly one translation unit.
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "card_textures.hpp"
#include <GLFW/glfw3.h>
#include <iostream>

// ---------------------------------------------------------------------------
// Filename mapping
// ---------------------------------------------------------------------------

std::string CardTextures::filename(const Card& c)
{
    std::string s;
    switch (c.rank) {
        case 14: s = "A";  break;
        case 13: s = "K";  break;
        case 12: s = "Q";  break;
        case 11: s = "J";  break;
        case 10: s = "10"; break;
        default: s = std::to_string(c.rank); break;
    }
    s += c.suit;   // 'C', 'D', 'H', or 'S'
    s += ".png";
    return s;
}

// ---------------------------------------------------------------------------
// Internal loader
// ---------------------------------------------------------------------------

GLuint CardTextures::loadOne(const std::string& path)
{
    int w, h, channels;
    unsigned char* data = stbi_load(path.c_str(), &w, &h, &channels, 4 /*force RGBA*/);
    if (!data) {
        std::cerr << "[CardTextures] failed to load \"" << path
                  << "\": " << stbi_failure_reason() << "\n";
        return 0;
    }

    GLuint tex = 0;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0,
                 GL_RGBA, GL_UNSIGNED_BYTE, data);
    stbi_image_free(data);

    // Record card dimensions from the first image loaded
    if (w_ == 71.f && h_ == 96.f) {
        w_ = static_cast<float>(w);
        h_ = static_cast<float>(h);
    }

    return tex;
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

bool CardTextures::load(const std::string& dir)
{
    std::string base = dir;
    if (!base.empty() && base.back() != '/')
        base += '/';

    bool allOk = true;

    const char suits[] = { 'C', 'D', 'H', 'S' };
    for (char suit : suits) {
        for (int rank = 2; rank <= 14; ++rank) {
            Card    c   { rank, suit };
            std::string fname = filename(c);
            GLuint  tex = loadOne(base + fname);
            if (tex)
                tex_[fname] = tex;
            else
                allOk = false;
        }
    }

    // Card back
    GLuint backTex = loadOne(base + "back.png");
    if (backTex)
        back_ = backTex;
    else
        allOk = false;

    return allOk;
}

void CardTextures::unload()
{
    for (auto& [key, tex] : tex_)
        glDeleteTextures(1, &tex);
    tex_.clear();

    if (back_) {
        glDeleteTextures(1, &back_);
        back_ = 0;
    }
}

GLuint CardTextures::get(const Card& c) const
{
    auto it = tex_.find(filename(c));
    return it != tex_.end() ? it->second : 0;
}
