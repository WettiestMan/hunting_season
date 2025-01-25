#include "raylib.h"
#include <cmath>
#include <string_view>
#include <algorithm>
#include <numbers>

#include "classes/base/IGameEntity.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/Duck.hpp"
#include "classes/Background.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

// path init (now that I think about it, I might make them constexpr and initialize them directly inside the header file)
const std::string_view Duck::texturePaths[Duck::spritesSize] = {R"(./assets/duck_spr_up.png)", R"(./assets/duck_spr_down.png)",
                                                                R"(./assets/duck_spr_dead_left.png)", R"(./assets/duck_spr_dead_right.png)"};

Duck::Duck() noexcept : IGameEntity(),
                        textureLoadError{false},
                        lastMove{ltime::steady_clock::now()},
                        lastFlap{ltime::steady_clock::now()},
                        movementFunctions{&movementFunctionConstant, &movementFunctionSine, &movementFunctionParabolic},
                        alive{true}
{
    Image imgs[spritesSize] = {0};
    for (size_t f = 0; f < spritesSize; ++f) {
        imgs[f] = LoadImage(texturePaths[f].data());
    }

    for (size_t i = 0; i < spritesSize; ++i) {
        sprites[i] = LoadTextureFromImage(imgs[i]);
    }

    for (size_t i = 0; i < spritesSize; ++i) {
        if (sprites[i].id <= 0) {
            TraceLog(LOG_WARNING, R"(Couldn't load asset "%s". Game will choose a box for the duck instead)", texturePaths[i].data());
            textureLoadError = true;
        }
        else {
            UnloadImage(imgs[i]);
        }
    }
    selectedRenderFunction = (!textureLoadError) ? &duckRender : &duckRender_ErrorCase;

    selectedSprite = GetRandomValue(0, 1);

    position.x = origin = originTable[GetRandomValue(0, 1)];
    position.y = GetRandomValue(0, Background::shootingAreaHeight - height);

    initialPosition = position;

    selectedMovement = movementFunctions[GetRandomValue(0, noMovements - 1)];
}

void Duck::update() noexcept {
    if (ltime::duration_cast<millis>(ltime::steady_clock::now() - lastFlap) >= timeBetweenSprites) {
        selectedSprite ^= 0b1;  // toggles between the 0th and 1st sprite (or the 2nd and 3rd sprite if )
        lastFlap = ltime::steady_clock::now();
    }

    (this->*selectedMovement)();
}

void Duck::render() noexcept {
    (this->*selectedRenderFunction)();
}

void Duck::kill() noexcept {
    alive = false;
    selectedSprite |= 0b10; // to select the last 2 sprites from the texture array
    selectedMovement = &movementFunctionDeath;
}

Vector2 Duck::getPosition() const noexcept {
    return position;
}

bool Duck::isAlive() const noexcept {
    return alive;
}

Duck::~Duck() noexcept {
    fn::for_each(sprites, [](Texture& tx){ if(tx.id >= 0) UnloadTexture(tx);});
}

// f(x) = yi
void Duck::movementFunctionConstant() noexcept {
    switch(origin) {
        case Origin::LEFT_SIDE:  // left to right
            position.x++;
            break;
        case Origin::RIGHT_SIDE:  //right to left
            position.x--;
            break;
    }
}

// f(x) = yi + 64sin(x);
void Duck::movementFunctionSine() noexcept {
    constexpr auto wavewidthController = 8 * std::numbers::pi_v<float>;

    switch(origin) {
        case Origin::LEFT_SIDE:
            position.y = initialPosition.y + (height / 2) * std::sin(position.x / wavewidthController);
            position.x++;
            break;
        case Origin::RIGHT_SIDE:
            position.y = initialPosition.y + (height / 2) * std::sin(position.x / wavewidthController);
            position.x--;
            break;
    }
}

// f(x) = 0.0002(x + xi)^2 + yi
void Duck::movementFunctionParabolic() noexcept {
    constexpr double K = 0.0002;

    switch (origin) {
        case Origin::LEFT_SIDE:
            position.y = K * (position.x * position.x) + initialPosition.y;
            position.x++;
            break;
        case Origin::RIGHT_SIDE:
            position.y = K * (position.x * position.x - Background::width) + initialPosition.y;
            position.x--;
            break;
    }
}

// f(y) = xf. Let xf be the x coordinate where the duck was killed
void Duck::movementFunctionDeath() noexcept {
    position.y--;
}

void Duck::duckRender() noexcept {
    DrawTexture(
        sprites[selectedSprite],
        static_cast<i32>(position.x),
        static_cast<i32>(position.y),
        WHITE
    );
}

void Duck::duckRender_ErrorCase() noexcept {
    DrawRectangle(
        static_cast<i32>(position.x),
        static_cast<i32>(position.y),
        width,
        height,
        BROWN
    );
}