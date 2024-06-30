#ifndef WATERFALL_CPP
#define WATERFALL_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class WaterfallShader : public smash::FragmentShader
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

    float mix(float a, float b, float t) const {
        return a * (1.0f - t) + b * t;
    }

public:
    void fragment(size_t x, size_t y, color& _color) const override
    {
        vec2 pos = vec2(0.0f, 0.0f);
        vec2 scal = vec2(64.0f, 32.0f);  // Adjusted for 64x32 pixel matrix
        
        float time = smash::Time::getRunningTime() * 0.4f;
        
        // Calculate normalized coordinates
        float nx = (float)(x - pos.x) / scal.x;
        float ny = (float)(y - pos.y) / scal.y;
        
        // Pixelate effect
        nx = std::floor(nx * 16.0f) / 16.0f;
        ny = std::floor(ny * 8.0f) / 8.0f;
        
        // Simulate displacement
        float displace = noise(nx * 4.0f, (ny + time) * 0.2f) * 0.1f;
        
        // Color calculation
        float noiseVal = noise((nx + displace) * 4.0f, (ny * 0.2f + time) * 2.0f);
        noiseVal = std::floor(noiseVal * 5.0f) / 5.0f;  // Quantize for color banding effect
        
        // Define colors
        float r1 = 0.510f, g1 = 0.776f, b1 = 0.486f;
        float r2 = 0.200f, g2 = 0.604f, b2 = 0.318f;
        float r3 = 0.145f, g3 = 0.490f, b3 = 0.278f;
        float r4 = 0.059f, g4 = 0.255f, b4 = 0.251f;
        
        // Mix colors
        float r = mix(mix(r1, r2, ny), mix(r3, r4, ny), noiseVal);
        float g = mix(mix(g1, g2, ny), mix(g3, g4, ny), noiseVal);
        float b = mix(mix(b1, b2, ny), mix(b3, b4, ny), noiseVal);
        
        // Add gradients
        float invY = 1.0f - ny;
        r -= 0.45f * std::pow(ny, 4.0f);
        g -= 0.45f * std::pow(ny, 4.0f);
        b -= 0.45f * std::pow(ny, 4.0f);
        r += std::pow(invY, 4.0f);
        g += std::pow(invY, 4.0f);
        b += std::pow(invY, 4.0f);
        
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