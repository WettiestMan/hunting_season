#include "raylib.h"
#include <algorithm>
#include <chrono>
#include <string_view>
#include <optional>

#include "classes/base/IGameEntity.hpp"
#include "classes/Crosshair.hpp"
#include "classes/Game.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

// path init
const std::string_view Crosshair::texturePath = R"(./assets/chair_red.png)";
const std::string_view Crosshair::blast1TexturePath = R"(./assets/blast1.png)";
const std::string_view Crosshair::blast2TexturePath = R"(./assets/blast2.png)";

Crosshair::Crosshair(Game& g) noexcept : IGameEntity(),
                                    shotTriggered{false},
                                    blastLoadError{false},
                                    position{0, 0},
                                    lastPressedPosition{0, 0},
                                    blastRenderRoutines{&blastRender, &blastRender_ErrorCase},
                                    game{g}
{
    auto textureImg = LoadImage(texturePath.data());
    Image impactImages[framesSize] = {LoadImage(blast1TexturePath.data()), LoadImage(blast2TexturePath.data())};

    cross = LoadTextureFromImage(textureImg);
    for (size_t i = 0; i < framesSize; ++i) {
        blasts[i] = LoadTextureFromImage(impactImages[i]);
    }

    if (cross.id <= 0) {
        TraceLog(LOG_WARNING, R"(Couldn't load asset "chair_red.png". Game will choose a dot for the crosshair instead)");
    }
    else {
        UnloadImage(textureImg);
    }

    for (size_t i = 0; i < framesSize; ++i) {
        if (blasts[i].id <= 0) {
            TraceLog(LOG_WARNING, R"(Either "blast1.png" or "blast2.png" (or both) couldn't be loaded. Game will use a plain color for the impact instead)");
            blastLoadError = true;
            break;
        }
        else {
            UnloadImage(impactImages[i]);  // I was having a memleak here (scari D:)
        }
    }
}

void Crosshair::update() noexcept {
    position = GetMousePosition();
    shotTriggered = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (shotTriggered) {
        shotTriggeredInstant = stopwatch::now();
        lastPressedPosition = position;
        blastSelectedTexture = static_cast<u8>(GetRandomValue(0, 1));
        game.setRegisteredShot(position);
        game.substractBullets(1);
    }
    else {
        game.setRegisteredShot(std::nullopt);
    }
}

void Crosshair::render() noexcept {
    if (cross.id > 0) {
        DrawTexture(cross,
            static_cast<i32>(position.x) - (okWidth / 2),
            static_cast<i32>(position.y) - (okHeight / 2),
            WHITE);
    }
    else {
        DrawCircle(static_cast<i32>(position.x), static_cast<i32>(position.y), backupWidth / 2, RED);
    }

    if (ltime::duration_cast<millis>(stopwatch::now() - shotTriggeredInstant) < blastFrameDisplayTime) {
        // Do you think I'm too scared of branching?
        (this->*blastRenderRoutines[blastLoadError])();
    }
}

Crosshair::~Crosshair() noexcept {
    if (cross.id > 0) {
        UnloadTexture(cross);
    }
    fn::for_each(blasts, [](Texture& tx){ if (tx.id > 0) UnloadTexture(tx); });
}

// private functions
void Crosshair::blastRender() noexcept {
    DrawTexture(
        blasts[blastSelectedTexture],
        static_cast<i32>(lastPressedPosition.x) - (blastWidth / 2),
        static_cast<i32>(lastPressedPosition.y) - (blastHeight / 2),
        WHITE
    );
}

void Crosshair::blastRender_ErrorCase() noexcept {
    DrawRectangle(
        static_cast<i32>(lastPressedPosition.x) - (blastWidth / 2),
        static_cast<i32>(lastPressedPosition.y) - (blastHeight / 2),
        blastWidth,
        blastHeight,
        YELLOW
    );
}