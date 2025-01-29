#include "raylib.h"

#include <list>
#include <algorithm>

#include "classes/base/IGameEntity.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/DuckManager.hpp"
#include "classes/Game.hpp"
#include "classes/Duck.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

DuckManager::DuckManager(Game& g) : IGameEntity(),
                            game{g},
                            spawnRate{defaultSpawnRate}
{
    ducks.emplace_back();
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
        ducks.emplace_back();
        lastSpawned = std::move(currentTime);
    }
}

void DuckManager::render() noexcept {
    for (auto& duck : ducks) {
        duck.update();
        duck.render();
    }
}

DuckManager::~DuckManager() {
    // Let's let RAII do its magic.
}