#pragma once

#include "mathf.hpp"
#include "types.hpp"

class Color {
   public:
    uint8 red = 0;
    uint8 green = 0;
    uint8 blue = 0;
    uint8 alpha = 255;

    static Color lerp(Color start, Color end, float percentage);
};

// Linear interpolation between two colors
Color Color::lerp(Color start, Color end, float percentage){

    // Interpolate each color separately
    uint8 red = Math::lerp(start.red, end.red, percentage);
    uint8 green = Math::lerp(start.green, end.green, percentage);
    uint8 blue = Math::lerp(start.blue, end.blue, percentage);
    uint8 alpha = Math::lerp(start.alpha, end.alpha, percentage);

    // Return a new Color
    return {red, green, blue, alpha};
}