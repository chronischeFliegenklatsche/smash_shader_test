#ifndef _GAME_RUNTIME_HPP
#define _GAME_RUNTIME_HPP
#include <smash.h>

class GameRuntime : public smash::Runtime {
public:
    void pipe() const override;
    GameRuntime();
    ~GameRuntime() override;
};

void _SMASH_GAME_LIFETIME();

#endif