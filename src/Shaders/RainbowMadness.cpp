#ifndef RAINBOW_MADNESS_CPP
#define RAINBOW_MADNESS_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class RainbowMadness : public smash::FragmentShader
{
private:
    void hue_to_rgb(float h, float& r, float& g, float& b) const {
        float h_prime = std::fmod(h, 1.0f) * 6.0f;
        float x = 1.0f - std::abs(std::fmod(h_prime, 2.0f) - 1.0f);
        
        if (h_prime < 1.0f) { r = 1.0f; g = x; b = 0.0f; }
        else if (h_prime < 2.0f) { r = x; g = 1.0f; b = 0.0f; }
        else if (h_prime < 3.0f) { r = 0.0f; g = 1.0f; b = x; }
        else if (h_prime < 4.0f) { r = 0.0f; g = x; b = 1.0f; }
        else if (h_prime < 5.0f) { r = x; g = 0.0f; b = 1.0f; }
        else { r = 1.0f; g = 0.0f; b = x; }
    }

    float fract(float x) const {
        return x - std::floor(x);
    }

public:
    void fragment(size_t x, size_t y, color& _color) const override
    {
        float width = 64.0f;   // Adjusted for 64x32 pixel matrix
        float height = 32.0f;  // Adjusted for 64x32 pixel matrix
        
        float time = smash::Time::getRunningTime();
        
        // Calculate position
        float nx = static_cast<float>(x) / width;
        float ny = static_cast<float>(y) / height;
        
        // Parameters (you can adjust these)
        float speed = 0.5f;
        float angle = 45.0f;  // in degrees
        
        // Calculate hue
        float hue = nx * std::cos(angle * 3.14159f / 180.0f) - ny * std::sin(angle * 3.14159f / 180.0f);
        hue = fract(hue + fract(time * speed));
        
        // Convert hue to RGB
        float r, g, b;
        hue_to_rgb(hue, r, g, b);
        
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