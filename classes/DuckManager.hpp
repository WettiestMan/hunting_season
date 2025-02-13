#ifndef DUCK_GENERATOR_HPP_HNSS
#define DUCK_GENERATOR_HPP_HNSS

#include "raylib.h"

#include <list>
#include <string_view>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Duck.hpp"
#include "Game.hpp"

class DuckManager : public IGameEntity {
public:
    constexpr static size_t duckSpritesSize = 4;

private:
    Game& game;
    std::list<Duck> ducks;
    instant lastSpawned;
    millis spawnRate;
    Texture duckSprites[duckSpritesSize];
    static const std::string_view duckSpritesPaths[duckSpritesSize]; // the first two are the flying sprites, the last two (coming soon) are the dying sprites
    bool spritesLoadError;

    static constexpr millis defaultSpawnRate {2000};
    static constexpr u32 outOfScreenSafeZone = 20;

public:
    DuckManager(Game& g);
    void update() override;
    void render() noexcept override;
    ~DuckManager() noexcept override;

private:
    bool isDuckOutOfScreen(typename decltype(ducks)::iterator duck);

};

#endif