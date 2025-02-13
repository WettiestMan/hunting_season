#include "raylib.h"

#include <list>
#include <algorithm>
#include <string_view>
#include <chrono>
#include <utility>

#include "classes/base/IGameEntity.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/DuckManager.hpp"
#include "classes/Game.hpp"
#include "classes/Duck.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

// path init (now that I think about it, I might make them constexpr and initialize them directly inside the header file)
const std::string_view DuckManager::duckSpritesPaths[DuckManager::duckSpritesSize] = {R"(./assets/duck_spr_up.png)", R"(./assets/duck_spr_down.png)",
                                                                R"(./assets/duck_spr_dead_left.png)", R"(./assets/duck_spr_dead_right.png)"};

DuckManager::DuckManager(Game& g) : IGameEntity(),
                            game{g},
                            spawnRate{defaultSpawnRate},
                            spritesLoadError{false}
{
    Image imgs[duckSpritesSize] = {0};
    for (size_t f = 0; f < duckSpritesSize; ++f) {
        imgs[f] = LoadImage(duckSpritesPaths[f].data());
    }

    for (size_t f = 0; f < duckSpritesSize; ++f) {
        duckSprites[f] = LoadTextureFromImage(imgs[f]);
    }

    for (size_t i = 0; i < duckSpritesSize; ++i) {
        if (duckSprites[i].id <= 0) {
            spritesLoadError = true;
            TraceLog(LOG_WARNING, R"(Couldn't load asset "%s". Game will choose a box for the duck instead)", duckSpritesPaths[i].data());
        }
        else UnloadImage(imgs[i]);
    }

    ducks.emplace_back((!spritesLoadError) ? duckSprites : nullptr);
    lastSpawned = stopwatch::now();
}

void DuckManager::update() {
    const auto possibleShot = game.getRegisteredShot();
    if (possibleShot.has_value()) {
        const auto shotCoords = *possibleShot;

        auto duck = ducks.begin();
        const auto finish = ducks.end();
        while(duck != finish) {
            const auto duckPos = duck->getPosition();

            if ((shotCoords.x >= duckPos.x && shotCoords.x <= (duckPos.x + Duck::width))
                && (shotCoords.y >= duckPos.y && shotCoords.y <= (duckPos.y + Duck::height))
                && duck->isAlive())
            {
                duck->kill();
                game.addPoints(1);
            }

            if (isDuckOutOfScreen(duck)) {
                duck = ducks.erase(duck);
            }
            else {
                ++duck;
            }
        }
    }
    else {  // Didn't think of a better approach
        auto duck = ducks.begin();
        const auto finish = ducks.end();
        while(duck != finish) {
            if (isDuckOutOfScreen(duck)) {
                duck = ducks.erase(duck);
            }
            else {
                ++duck;
            }
        }
    }

    if (auto currentTime = stopwatch::now(); ltime::duration_cast<millis>(currentTime - lastSpawned) >= spawnRate) {
        ducks.emplace_back((!spritesLoadError) ? duckSprites : nullptr);
        lastSpawned = std::move(currentTime);
    }
}

void DuckManager::render() noexcept {
    for (auto& duck : ducks) {
        duck.update();
        duck.render();
    }
}

bool DuckManager::isDuckOutOfScreen(typename decltype(ducks)::iterator duck) {
    const auto duckPos = duck->getPosition();

    const bool isOutOfScreen = ((duckPos.x <= (0 - (i32)Duck::width - (i32)outOfScreenSafeZone))
                                || (duckPos.x >= (Background::width + outOfScreenSafeZone))
                                || (duckPos.y <= (0 - (i32)Duck::height - (i32)outOfScreenSafeZone))
                                || (duckPos.y >= (Background::height + outOfScreenSafeZone)));

    return isOutOfScreen;
};

DuckManager::~DuckManager() {
    fn::for_each(duckSprites, [](Texture& tx){ if (tx.id > 0) UnloadTexture(tx); });
    // The rest is handled by RAII
}