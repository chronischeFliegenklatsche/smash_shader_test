// Define pin configuration
#define R1_PIN 25
#define G1_PIN 26
#define B1_PIN 27
#define R2_PIN 14
#define G2_PIN 12
#define B2_PIN 13
#define A_PIN 23
#define B_PIN 22
#define C_PIN 5
#define D_PIN 18
#define E_PIN -1  // Set to -1 as it's not needed for 64x32 panels
#define LAT_PIN 4
#define OE_PIN 15
#define CLK_PIN 2  // Changed from 16 to 2


#include <GameRuntime.hpp>
#include "Scenes/SampleScene.cpp"

GameRuntime::GameRuntime() : smash::Runtime() {
    // Set up game data and scene management
    std::shared_ptr<smash::Scene> mainScene = std::static_pointer_cast<smash::Scene>(std::make_shared<SampleScene>()); // replace nullptr with initial scene
    
    if (mainScene)
    {
        smash::SceneManagement::addScene(mainScene);
        smash::SceneManagement::setActiveScene(mainScene.get());
    }
    
    // Set up input API
    std::shared_ptr<smash::InputAPI> inputAPI;
#ifdef ESP32
    inputAPI = std::make_shared<smash::ArduinoInputAPI>();
#endif
#ifdef _WIN32
    inputAPI = std::make_shared<smash::GLInputAPI>();
#endif
    if (inputAPI)
    {
        smash::InputDetection::setInputAPI(inputAPI);
    }
    

    // Set up rendering API
    std::shared_ptr<smash::RenderingAPI> renderingAPI;
#ifdef ESP32
    // Configure matrix
    HUB75_I2S_CFG::i2s_pins _pins = {
        R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN,
        A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN
    };

    HUB75_I2S_CFG mxconfig(
        64,   // Module width
        32,   // Module height
        1,    // Chain length
        _pins // Pin mapping
    );
    renderingAPI = std::make_shared<smash::RgbMatrixRenderingAPI>(mxconfig);
#endif


#ifdef _WIN32
    renderingAPI = std::make_shared<smash::GLRenderingAPI>();
    
#endif
    if (renderingAPI)
    {
        smash::Rendering::setRenderingAPI(renderingAPI);
    }
    
}

void GameRuntime::pipe() const {
    // Provide default pipeline
    smash::Runtime::pipe();
}

GameRuntime::~GameRuntime() {
    // Cleanup if necessary
}

void _SMASH_GAME_LIFETIME()
{
    GameRuntime gameRuntime;
    _SMASH_ENGINE_LOOP(gameRuntime);
}