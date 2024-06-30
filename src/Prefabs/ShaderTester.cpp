#ifndef _SHADER_TEST_CPP
#define _SHADER_TEST_CPP

#include <smash.h>
#include "../Shaders/RainbowMadness.cpp"
#include "../Shaders/PurpleBubbles.cpp"
#include "../Shaders/GlitchSky.cpp"
#include "../Shaders/CosmicNebula.cpp"
#include "../Shaders/RainbowRing.cpp"
#include "../Shaders/Waterfall.cpp"

class ShaderTester : public smash::GameObject {    

public:

    ShaderTester() : GameObject() {

        // Initialize shader renderer
        auto shadRenderer = std::make_shared<smash::ShaderRenderer>();

        // Initialize shader attributes
        auto shadAttributes = std::make_unique<smash::ShaderAttributes>();
        
        // Set shader attributes
        shadRenderer->bindStartFrameShaderAttributes(std::move(shadAttributes));

        // Set shader
        auto shader = std::unique_ptr<smash::Shader>(new RainbowMadness());
        shadRenderer->bind(std::move(shader));

        // Add component
        addComponent(shadRenderer);
    }

};

#endif