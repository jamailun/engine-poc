#pragma once

#include <stdint.h>

struct SDL_Color;

namespace engine {

/**
 * Defines a structure
 */
struct Color {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    SDL_Color to_sdl() const;
};

/**
 * Colors.
 */
namespace colors {
    const Color none        {0, 0, 0, 0};

    const Color black       {0, 0, 0, 255};
    const Color white       {255, 255, 255, 255};
    const Color alpha_white {255, 255, 255, 100};

    const Color gray        {127, 127, 127, 255};
    const Color light_gray  {192, 192, 192, 255};
    const Color dark_gray   {63, 63, 63, 255};

    const Color red         {255, 0, 0, 255};
    const Color green       {0, 255, 0, 255};
    const Color blue        {0, 0, 255, 255};

    const Color light_green {96, 255, 128, 255};
    const Color light_blue  {80, 208, 32, 255};
    const Color pink        {255, 96, 208, 255};

    const Color purple      {160, 32, 255, 255};
    const Color orange      {255, 224, 32, 255};
    const Color yellow      {255, 160, 16, 255};
    const Color brown       {160, 128, 96, 255};

    Color semi_random(int id);
} // namespace colors

} // namespace engine
