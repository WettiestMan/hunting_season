#ifndef DUCK_GENERATOR_HPP_HNSS
#define DUCK_GENERATOR_HPP_HNSS

#include "raylib.h"

#include <list>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Duck.hpp"
#include "Game.hpp"

class DuckManager : public IGameEntity {
private:
    Game& game;
    std::list<Duck> ducks;
    instant lastSpawned;
    millis spawnRate;

    static constexpr millis defaultSpawnRate {2000};
    static constexpr u32 outOfScreenSafeZone = 20;

public:
    DuckManager(Game& g);
    void update() override;
    void render() noexcept override;
    ~DuckManager() noexcept override;

private:
    inline bool isDuckOutOfScreen(typename decltype(ducks)::iterator duck) {
        const auto duckPos = duck->getPosition();

        const bool isOutOfScreen = ((duckPos.x <= (0 - (i32)Duck::width - (i32)outOfScreenSafeZone))
                                   || (duckPos.x >= (Background::width + outOfScreenSafeZone))
                                   || (duckPos.y <= (0 - (i32)Duck::height - (i32)outOfScreenSafeZone))
                                   || (duckPos.y >= (Background::height + outOfScreenSafeZone)));

        return isOutOfScreen;
    };

    
};

#endif