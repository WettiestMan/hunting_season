#include "raylib.h"
#include <algorithm>
#include <chrono>
#include <string_view>

#include "classes/base/IGameEntity.hpp"
#include "classes/Crosshair.hpp"

namespace ltime = std::chrono;
namespace fn = std::ranges;

// path init
const std::string_view Crosshair::texturePath = R"(./assets/chair_red.png)";
const std::string_view Crosshair::blast1TexturePath = R"(./assets/blast1.png)";
const std::string_view Crosshair::blast2TexturePath = R"(./assets/blast2.png)";

Crosshair::Crosshair() noexcept : IGameEntity(),
                                    shotTriggered{false},
                                    blastLoadError{false},
                                    showBlastFrames{false},
                                    position{0, 0},
                                    lastPressedPosition{0, 0},
                                    blastRenderRoutines{&blastRender, &blastRender_ErrorCase} {
    auto textureImg = LoadImage(texturePath.data());
    Image impactImages[2] = {LoadImage(blast1TexturePath.data()), LoadImage(blast2TexturePath.data())};

    cross = LoadTextureFromImage(textureImg);
    for (size_t i = 0; i < blastRenderRoutinesSize; ++i) {
        blasts[i] = LoadTextureFromImage(impactImages[i]);
    }

    if (cross.id <= 0) {
        TraceLog(LOG_WARNING, R"(Couldn't load asset "chair_red.png". Game will choose a dot for the crosshair instead)");
        width = backupWidth;
        height = backupHeight;
    }
    else {
        UnloadImage(textureImg);
        width = okWidth;
        height = okHeight;
    }

    for (size_t i = 0; i < blastRenderRoutinesSize; ++i) {
        if (blasts[i].id <= 0) {
            TraceLog(LOG_WARNING, R"(Either "blast1.png" or "blast2.png" (or both) couldn't be loaded. Game will use a plain color for the impact instead)");
            blastLoadError = true;
            break;
        }
    }
}

void Crosshair::update() noexcept {
    position = GetMousePosition();
    shotTriggered = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);

    if (shotTriggered) {
        shotTriggeredInstant = ltime::system_clock::now();
        showBlastFrames = true;
        lastPressedPosition = position;
        blastSelectedTexture = static_cast<u8>(GetRandomValue(0, 1));
    }
}

void Crosshair::render() noexcept {
    if (cross.id > 0) {
        DrawTexture(cross,
            static_cast<i32>(position.x) - (width >> 1),
            static_cast<i32>(position.y) - (height >> 1),
            WHITE);
    }
    else {
        DrawCircle(static_cast<i32>(position.x), static_cast<i32>(position.y), width >> 1, RED);
    }

    if (ltime::duration_cast<millis>(ltime::system_clock::now() - shotTriggeredInstant) < blastFrameDisplayTime) {
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
        static_cast<i32>(lastPressedPosition.x) - (blastWidth >> 1),
        static_cast<i32>(lastPressedPosition.y) - (blastHeight >> 1),
        WHITE
    );
}

void Crosshair::blastRender_ErrorCase() noexcept {
    DrawRectangle(
        static_cast<i32>(lastPressedPosition.x) - (blastWidth >> 1),
        static_cast<i32>(lastPressedPosition.y) - (blastHeight >> 1),
        blastWidth,
        blastHeight,
        YELLOW
    );
}