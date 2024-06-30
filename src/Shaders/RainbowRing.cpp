#ifndef _RAINBOW_RING_CPP
#define _RAINBOW_RING_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class RainbowRing : public smash::FragmentShader
{
private:
    float atan2(float y, float x) const {
        return std::atan2(y, x);
    }

    float length(float x, float y) const {
        return std::sqrt(x*x + y*y);
    }

    float clamp(float x, float min, float max) const {
        return std::fmax(min, std::fmin(max, x));
    }

public:
    void fragment(size_t x, size_t y, color& _color) const override
    {
        vec2 pos = vec2(0.0f, 0.0f);
        vec2 scal = vec2(64.0f, 32.0f);  // Adjusted for 64x32 pixel matrix
        
        float time = smash::Time::getRunningTime();
        
        // Calculate normalized coordinates
        float nx = (float)(x - pos.x) / scal.x - 0.5f;
        float ny = (float)(y - pos.y) / scal.y - 0.5f;
        
        // Adjust aspect ratio
        nx *= scal.x / scal.y;
        
        // Calculate polar coordinates
        float radius = length(nx, ny) - 0.3f;
        float angle = atan2(ny, nx);
        
        // Generate rainbow colors
        float r = 0.5f + 0.5f * std::cos(angle + time + 0.0f);
        float g = 0.5f + 0.5f * std::cos(angle + time + 2.094f);
        float b = 0.5f + 0.5f * std::cos(angle + time + 4.188f);
        
        // Create ring effect
        float ringWidth = 0.1f;
        float ringEffect = clamp((ringWidth - std::abs(radius)) / ringWidth, 0.0f, 1.0f);
        
        // Apply ring effect to colors
        r *= ringEffect;
        g *= ringEffect;
        b *= ringEffect;
        
        // Add pulsating effect
        float pulse = 0.5f + 0.5f * std::sin(time * 2.0f);
        r *= 0.5f + 0.5f * pulse;
        g *= 0.5f + 0.5f * pulse;
        b *= 0.5f + 0.5f * pulse;
        
        // Ensure colors are within [0, 1] range
        r = clamp(r, 0.0f, 1.0f);
        g = clamp(g, 0.0f, 1.0f);
        b = clamp(b, 0.0f, 1.0f);
        
        // Convert float [0, 1] to uint8_t [0, 255]
        uint8_t r_uint8 = static_cast<uint8_t>(r * 255.0f);
        uint8_t g_uint8 = static_cast<uint8_t>(g * 255.0f);
        uint8_t b_uint8 = static_cast<uint8_t>(b * 255.0f);
        
        // Set the final color
        _color = color(r_uint8, g_uint8, b_uint8);
    }
};

#endif