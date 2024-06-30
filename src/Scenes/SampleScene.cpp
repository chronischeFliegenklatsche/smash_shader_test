#ifndef _SAMPLE_SCENE_CPP
#define _SAMPLE_SCENE_CPP

#include <smash.h>
#include "../Prefabs/ShaderTester.cpp"

class SampleScene : public smash::Scene
{
public:
    SampleScene() : Scene()
    {
        // Create a new shader tester
        auto shaderTester = std::make_shared<ShaderTester>();
        instantiate(shaderTester);
    }
};

#endif