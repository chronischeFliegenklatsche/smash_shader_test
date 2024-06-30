#ifndef _RAINBOW_MADNESS_CPP
#define _RAINBOW_MADNESS_CPP

#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class RainbowMadness : public smash::FragmentShader {

protected:
    void fragment(size_t x, size_t y, color &_color) const override
    {
        float time = smash::Time::getRunningTime();
        
        // Normalize x to a value between 0 and 1
        float normalizedX = static_cast<float>(x) / static_cast<float>(1.000f);

        // Calculate the hue based on the normalized x position and time
        float hue = std::fmod(normalizedX + time * 0.1f, 1.0f);

        // Convert hue to RGB
        float r, g, b;
        hueToRGB(hue, r, g, b);

        // Convert RGB values to the range [0, 255]
        _color = color(static_cast<int>(r * 255), static_cast<int>(g * 255), static_cast<int>(b * 255));
    }

    // Helper function to convert hue to RGB
    void hueToRGB(float hue, float &r, float &g, float &b) const
    {
        float s = 1.0f; // Saturation
        float v = 1.0f; // Value

        int i = static_cast<int>(hue * 6);
        float f = hue * 6 - i;
        float p = v * (1 - s);
        float q = v * (1 - f * s);
        float t = v * (1 - (1 - f) * s);

        switch (i % 6) {
            case 0: r = v; g = t; b = p; break;
            case 1: r = q; g = v; b = p; break;
            case 2: r = p; g = v; b = t; break;
            case 3: r = p; g = q; b = v; break;
            case 4: r = t; g = p; b = v; break;
            case 5: r = v; g = p; b = q; break;
        }
    }
};

#endif