#include "raylib.h"

#include <cmath>
#include <chrono>
#include <string_view>
#include <algorithm>
#include <numbers>

#include "classes/base/IGameEntity.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/BulletManager.hpp"
#include "classes/Background.hpp"

namespace ltime = std::chrono;

BulletPack::BulletPack(const Texture* txs) noexcept : IGameEntity(),
                                        textureLoadError{false},
                                        lastMove{stopwatch::now()},
                                        lastSpriteSwitch{stopwatch::now()},
                                        alive{true},
                                        spritesBufferPtr{txs},
                                        selectedSprite{0},
                                        spawnInstant{stopwatch::now()}
{
    selectedRenderFunction = (txs != nullptr) ? &bpackRender : &bpackRender_ErrorCase;

    position.x = GetRandomValue(0, Background::width);
    position.y = Background::height;

    initialPosition = position;
}

void BulletPack::update() noexcept {
    constexpr float gravity = 1.0f/2250.0f; // ~4.4e-4, the measure is in px/ms^2
    constexpr i64 peakTime = 1500; // 1500 ms

    if (ltime::duration_cast<millis>(stopwatch::now() - lastSpriteSwitch) >= timeBetweenSprites) {
        if (selectedSprite < 3)
            ++selectedSprite;
        else
            selectedSprite = 0;

        lastSpriteSwitch = stopwatch::now();
    }

    // Formula:
    // y = (1/2)*gravity*(t - peaktime)^2 + 220
    const i64 currTimeOnScreen = ltime::duration_cast<millis>(stopwatch::now() - spawnInstant).count();
    position.y = 0.5f * gravity * std::pow(currTimeOnScreen - peakTime, 2) + Background::shootingAreaHeight / 2;
}

void BulletPack::render() noexcept {
    (this->*selectedRenderFunction)();
}

void BulletPack::bpackRender() noexcept {
    DrawTexture(
        spritesBufferPtr[selectedSprite],
        static_cast<i32>(position.x),
        static_cast<i32>(position.y),
        WHITE
    );
}

void BulletPack::bpackRender_ErrorCase() noexcept {
    DrawRectangle(
        static_cast<i32>(position.x),
        static_cast<i32>(position.y),
        width,
        height,
        Color{75, 83, 32, 255}
    );
}

void BulletPack::kill() noexcept {
    alive = false;
}

bool BulletPack::isAlive() const noexcept {
    return alive;
}

Vector2 BulletPack::getPosition() const noexcept {
    return position;
}

BulletPack::~BulletPack() noexcept {
    // there goes nothing here lololol
}