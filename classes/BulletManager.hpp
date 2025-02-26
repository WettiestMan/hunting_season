#ifndef BULLET_MANAGER_HPP_HNSS
#define BULLET_MANAGER_HPP_HNSS

#include "raylib.h"

#include <list>
#include <string_view>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "BulletPack.hpp"
#include "Game.hpp"

class BulletManager: public IGameEntity {
public:
    static constexpr size_t bpacksSpritesSize = 4;

private:
    Game& game;
    std::list<BulletPack> bulletPacks;
    instant lastSpawned;
    millis spawnRate;
    Texture sprites[bpacksSpritesSize];
    static const std::string_view spritesPaths[bpacksSpritesSize];
    bool spritesLoadError;

    static constexpr millis defaultSpawnRate {6000};
    static constexpr u32 outOfScreenSafeZone = 20;

public:
    BulletManager(Game& g);
    void render() override;
    void update() noexcept override;
    ~BulletManager() noexcept override;

private:
    bool isBpackOutOfScreen(typename decltype(bulletPacks)::iterator bpack) const noexcept;
};

#endif