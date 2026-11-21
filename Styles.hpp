#pragma once

#include <array>
#include <cstddef>

namespace ytautodeco {

struct Style {
    const char* name;
    // Unique style value controls motif, density, and rotation variation.
    unsigned char pattern;
    bool swapChannels;
};

inline constexpr std::array<Style, 33> STYLES{{
    {"Nightmare", 0, false},
    {"Voltage", 1, true},
    {"Ice", 2, false},
    {"Blood", 3, true},
    {"Hell", 4, false},
    {"Polar", 5, true},
    {"Inferno", 6, false},
    {"Skyline", 7, true},
    {"Neon", 8, false},
    {"Aqua", 9, true},
    {"Dessert", 10, false},
    {"Forest", 11, true},
    {"Candy", 12, false},
    {"Cyber", 13, true},
    {"Toxic", 14, false},
    {"Lava", 15, true},
    {"Galaxy", 16, false},
    {"Sunset", 17, true},
    {"Arctic", 18, false},
    {"Shadow", 19, true},
    {"Crystal", 20, false},
    {"Techno", 21, true},
    {"Ocean", 22, false},
    {"Jungle", 23, true},
    {"Royal", 24, false},
    {"Retro", 25, true},
    {"Glitch", 26, false},
    {"Plasma", 27, true},
    {"Sakura", 28, false},
    {"Desert Storm", 29, true},
    {"Mythic", 30, false},
    {"Void", 31, true},
    {"Classic", 32, false},
}};

inline constexpr std::size_t STYLE_COUNT = STYLES.size();

} // namespace ytautodeco
