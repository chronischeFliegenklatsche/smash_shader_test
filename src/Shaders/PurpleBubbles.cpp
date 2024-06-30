#ifndef _PURPLEBUBBLES_CPP
#define _PURPLEBUBBLES_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class Gold : public smash::FragmentShader
{
    void fragment(size_t x, size_t y, color& _color) const override
    {
        float time = smash::Time::getRunningTime();
        
        // Calculate normalized coordinates
        float nx = static_cast<float>(x) / 1000.0f; // Assuming a width of 1000, adjust if needed
        float ny = static_cast<float>(y) / 1000.0f; // Assuming a height of 1000, adjust if needed
        
        // Create a more complex shimmer effect
        float shimmer1 = std::sin(nx * 4 + ny * 3 + time * 1.2f) * 0.5f + 0.5f;
        float shimmer2 = std::cos(ny * 5 - nx * 2 + time * 0.8f) * 0.5f + 0.5f;
        float shimmer = (shimmer1 + shimmer2) * 0.5f;
        
        // Base golden yellow color (reduced green)
        float r = 1.0f;
        float g = 0.9f;
        float b = 0.1f;
        
        // Apply shimmer effect with reduced variation
        r = r * (0.95f + shimmer * 0.05f);
        g = g * (0.95f + shimmer * 0.05f);
        b = b * (0.9f + shimmer * 0.1f);
        
        // Ensure colors are within [0, 1] range
        r = std::fmax(0.0f, std::fmin(1.0f, r));
        g = std::fmax(0.0f, std::fmin(1.0f, g));
        b = std::fmax(0.0f, std::fmin(1.0f, b));
        
        // Convert float [0, 1] to uint8_t [0, 255]
        uint8_t r_uint8 = static_cast<uint8_t>(r * 255.0f);
        uint8_t g_uint8 = static_cast<uint8_t>(g * 255.0f);
        uint8_t b_uint8 = static_cast<uint8_t>(b * 255.0f);
        
        // Set the final color
        _color = color(r_uint8, g_uint8, b_uint8);
    }
};

#endif