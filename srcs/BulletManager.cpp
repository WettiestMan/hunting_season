#include "raylib.h"

#include <chrono>
#include <algorithm>
#include <utility>

#include "classes/BulletManager.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/Background.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

const std::string_view BulletManager::spritesPaths[BulletManager::bpacksSpritesSize] = {R"(./assets/bullet_pack_left.png)",
                                                        R"(./assets/bullet_pack_up.png)", R"(./assets/bullet_pack_down.png)",
                                                        R"(./assets/bullet_pack_right.png)"};

BulletManager::BulletManager(Game& g) : IGameEntity(),
                                        game{g},
                                        spawnRate{defaultSpawnRate},
                                        spritesLoadError{false}
{
    Image imgs[bpacksSpritesSize] = {0};
    for (size_t i = 0; i < bpacksSpritesSize; ++i) {
        imgs[i] = LoadImage(spritesPaths[i].data());
    }

    for (size_t f = 0; f < bpacksSpritesSize; ++f) {
        sprites[f] = LoadTextureFromImage(imgs[f]);
    }

    for (size_t i = 0; i < bpacksSpritesSize; ++i) {
        if (sprites[i].id <= 0) {
            spritesLoadError = true;
            TraceLog(LOG_WARNING, R"(Couldn't load asset "%s". Game will choose a box for the bullet pack instead)", spritesPaths[i].data());
        }
        else UnloadImage(imgs[i]);
    }

    bulletPacks.emplace_back((!spritesLoadError) ? sprites : nullptr);
    lastSpawned = stopwatch::now();
}

void BulletManager::update() {
    const auto possibleShot = game.getRegisteredShot();

    if  (possibleShot.has_value()) {
        const auto shotCoords = *possibleShot;

        auto bpack = bulletPacks.begin();
        const auto finish = bulletPacks.end();

        while (bpack != finish) {
            const auto bpackPos = bpack->getPosition();

            if ((shotCoords.x >= bpackPos.x && shotCoords.x <= (bpackPos.x + BulletPack::width))
                && (shotCoords.y >= bpackPos.y && shotCoords.y <= (bpackPos.y + BulletPack::height))
                && bpack->isAlive())
            {
                bpack->kill();
                game.resetBullets();
            }

            if (isBpackOutOfScreen(bpack) || !bpack->isAlive()) {
                bpack = bulletPacks.erase(bpack);
            }
            else {
                ++bpack;
            }
        }
    }
    else {
        auto bpack = bulletPacks.begin();
        const auto finish = bulletPacks.end();

        while (bpack != finish) {
            if (isBpackOutOfScreen(bpack) || !bpack->isAlive()) {
                bpack = bulletPacks.erase(bpack);
            }
            else {
                ++bpack;
            }
        }
    }

    if (auto currentTime = stopwatch::now(); ltime::duration_cast<millis>(currentTime - lastSpawned) >= spawnRate) {
        bulletPacks.emplace_back((!spritesLoadError) ? sprites : nullptr);
        lastSpawned = std::move(currentTime);
    }
}

void BulletManager::render() noexcept {
    for (auto& bpack : bulletPacks) {
        bpack.update();
        bpack.render();
    }
}

bool BulletManager::isBpackOutOfScreen(typename decltype(bulletPacks)::iterator bpack) const noexcept {
    const auto bpackPos = bpack->getPosition();

    const bool isOutOfScreen = ((bpackPos.x <= (0 - (i32)BulletPack::width - (i32)outOfScreenSafeZone))
                                || (bpackPos.x >= (Background::width + outOfScreenSafeZone))
                                || (bpackPos.y <= (0 - (i32)BulletPack::height - (i32)outOfScreenSafeZone))
                                || (bpackPos.y >= (Background::height + outOfScreenSafeZone)));

    return isOutOfScreen;
}

BulletManager::~BulletManager() noexcept {
    fn::for_each(sprites, [](Texture& tx){ if (tx.id > 0) UnloadTexture(tx); });
}