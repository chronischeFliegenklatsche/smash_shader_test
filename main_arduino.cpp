
#include <GameRuntime.hpp>

#ifdef ESP32
#include <Arduino.h>

void setup() {
    _SMASH_GAME_LIFETIME();
}

void loop() {
    if (!Serial)
    {
        Serial.begin(115200);
    }
    Serial.println("Smash game lifetime has been terminated.");
    Serial.println("Press reset button on you ESP32 to restart the game.");
    delay(3000);
}
#endif