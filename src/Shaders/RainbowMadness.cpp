#ifndef _RAINBOW_MADNESS_CPP
#define _RAINBOW_MADNESS_CPP

#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class RainbowMadness : public smash::FragmentShader
{
    void fragment(size_t x, size_t y, color& _color) const override
    {
        vec2 pos = *(vec2*)(gp("m_Position"));
        vec2 scal = *(vec2*)(gp("m_Scale"));
        
        float time = smash::Time::getRunningTime();
            
        // Calculate the relative x position within the rectangle
        float relativeX = (float)(x - pos.x) / scal.x;
        
        // Add time to make it move
        float movingX = relativeX + time * 0.5f; // Adjust the 0.5f to change speed
        
        // Use fmod to wrap the value between 0 and 1
        movingX = std::fmod(movingX, 1.0f);
        
        // Generate rainbow colors
        float r, g, b;
        
        if (movingX < 1.0f/6.0f) {
            r = 1.0f; g = movingX * 6.0f; b = 0.0f;
        } else if (movingX < 2.0f/6.0f) {
            r = (2.0f/6.0f - movingX) * 6.0f; g = 1.0f; b = 0.0f;
        } else if (movingX < 3.0f/6.0f) {
            r = 0.0f; g = 1.0f; b = (movingX - 2.0f/6.0f) * 6.0f;
        } else if (movingX < 4.0f/6.0f) {
            r = 0.0f; g = (4.0f/6.0f - movingX) * 6.0f; b = 1.0f;
        } else if (movingX < 5.0f/6.0f) {
            r = (movingX - 4.0f/6.0f) * 6.0f; g = 0.0f; b = 1.0f;
        } else {
            r = 1.0f; g = 0.0f; b = (1.0f - movingX) * 6.0f;
        }
        
        // Convert float [0, 1] to uint8_t [0, 255]
        uint8_t r_uint8 = static_cast<uint8_t>(r * 255.0f);
        uint8_t g_uint8 = static_cast<uint8_t>(g * 255.0f);
        uint8_t b_uint8 = static_cast<uint8_t>(b * 255.0f);

        // Assuming Color takes RGBA values with uint8_t channels
        _color = color(r_uint8, g_uint8, b_uint8); // 255 for full opacity
    }
            
};

#endif