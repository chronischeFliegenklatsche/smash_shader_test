#ifndef _GLITCHSKY_CPP
#define _GLITCHSKY_CPP
#include <smash.h>
#include <smash/sh.hpp>
#include <cmath>

class GlitchSky : public smash::FragmentShader
{
private:
    float lerp(float a, float b, float t) const {
        return a + t * (b - a);
    }

    float random(float x, float y) const {
        float dot = std::sin(x * 12.9898f + y * 78.233f) * 43758.5453f;
        return std::fmod(dot, 1.0f);
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

    float fbm(float x, float y, int octaves) const {
        float value = 0.0f;
        float amplitude = 0.5f;
        float frequency = 1.0f;
        for (int i = 0; i < octaves; ++i) {
            value += amplitude * noise(x * frequency, y * frequency);
            amplitude *= 0.5f;
            frequency *= 2.0f;
        }
        return value;
    }

    float glitchEffect(float x, float y, float time) const {
        float glitchIntensity = std::sin(time * 1.5f) * 0.5f + 0.5f;
        float glitchLine = std::floor(y * 8.0f) / 8.0f;  // Reduced lines for smaller display
        float glitchOffset = random(glitchLine, std::floor(time * 5.0f)) * 0.05f;
        
        x += glitchOffset * glitchIntensity;
        
        float glitchNoise = noise(x * 5.0f, y * 5.0f + time);
        return glitchNoise * glitchIntensity;
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
        
        // Create base sky gradient
        float skyGradient = ny * 1.2f - 0.1f;
        skyGradient = std::fmax(0.0f, std::fmin(1.0f, skyGradient));
        
        // Generate pattern using simplified FBM
        float pattern = fbm(nx * 3.0f + time * 0.05f, ny * 3.0f - time * 0.07f, 3);
        pattern = std::pow(pattern, 1.3f); // Slight contrast adjustment
        
        // Create pulsating effect
        float pulse = (std::sin(time * 0.3f) * 0.5f + 0.5f) * 0.15f + 0.85f;
        
        // Generate glitch effect
        float glitch = glitchEffect(nx, ny, time);
        
        // Combine effects
        float finalEffect = pattern * pulse + glitch * 0.3f;
        
        // Color mixing (adjusted for more vibrant colors on small display)
        float r = lerp(0.2f, 0.1f, skyGradient) + finalEffect * 0.4f;
        float g = lerp(0.1f, 0.3f, skyGradient) + finalEffect * 0.3f;
        float b = lerp(0.5f, 0.7f, skyGradient) + finalEffect * 0.2f;
        
        // Add simplified "electric" highlights
        float highlight = std::pow(noise(nx * 10.0f + time, ny * 10.0f - time), 4.0f);
        r += highlight * 0.3f;
        g += highlight * 0.2f;
        b += highlight * 0.4f;
        
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