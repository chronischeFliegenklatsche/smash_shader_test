#ifndef COSMIC_NEBULA_CPP
#define COSMIC_NEBULA_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class CosmicNebula : public smash::FragmentShader
{
private:
    float random(float x, float y) const {
        return std::fmod(std::sin(x * 12.9898f + y * 78.233f) * 43758.5453f, 1.0f);
    }

    float noise(float x, float y) const {
        float ix = std::floor(x);
        float iy = std::floor(y);
        float fx = x - ix;
        float fy = y - iy;
        
        float a = random(ix, iy);
        float b = random(ix + 1, iy);
        float c = random(ix, iy + 1);
        float d = random(ix + 1, iy + 1);
        
        float ux = fx * fx * (3.0f - 2.0f * fx);
        float uy = fy * fy * (3.0f - 2.0f * fy);
        
        return a * (1.0f - ux) * (1.0f - uy) + b * ux * (1.0f - uy) + c * (1.0f - ux) * uy + d * ux * uy;
    }

public:
    void fragment(size_t x, size_t y, color& _color) const override
    {
        vec2 pos = vec2(0.0f, 0.0f);
        vec2 scal = vec2(64.0f, 32.0f);  // Adjusted for 64x32 pixel matrix
        
        float time = smash::Time::getRunningTime();
        
        // Calculate normalized coordinates
        float nx = (float)(x - pos.x) / scal.x;
        float ny = (float)(y - pos.y) / scal.y;
        
        // Create nebula effect (adjusted scale for smaller display)
        float nebula = noise(nx * 4.0f + time * 0.1f, ny * 4.0f) * 
                       noise(nx * 6.0f - time * 0.07f, ny * 6.0f);
        nebula = std::pow(nebula, 2.0f);  // Increase contrast
        
        // Generate base colors (more blue, less white)
        float r = nebula * 0.2f;
        float g = nebula * 0.1f;
        float b = nebula * 0.5f + 0.2f;  // Increased base blue
        
        // Add simple star effect
        float star = std::pow(random(nx * 1000.0f, ny * 1000.0f), 20.0f);
        float twinkle = (std::sin(time * 3.0f + nx * 50.0f + ny * 50.0f) * 0.5f + 0.5f) * 0.7f + 0.3f;
        star *= twinkle;
        
        // Apply star to color (subtle, glowy white dots)
        float starIntensity = star * 0.5f;  // Reduced intensity
        r += starIntensity;
        g += starIntensity;
        b += starIntensity;
        
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