#include "raylib.h"

#include "classes/base/IGameEntity.hpp"
#include "classes/defs/TypeAliasses.hpp"
#include "classes/Background.hpp"

// path init
const std::string_view Background::texturePath = R"(./assets/bg.png)";

Background::Background() noexcept : IGameEntity() {
    auto img = LoadImage(texturePath.data());
    backgroundTexture = LoadTextureFromImage(img);
    if (backgroundTexture.id <= 0) {
        TraceLog(LOG_WARNING, "Couldn't load background asset \"bg.png\". Game will choose a plain color for the background instead.");
    }
    else {
        UnloadImage(img);
    }
    fieldSize = {0, 0, shootingAreaWidth, shootingAreaHeight};
}

void Background::update() noexcept {
    // background never changes... for now
}

void Background::render() noexcept {
    if (backgroundTexture.id > 0) {
        DrawTexture(backgroundTexture, 0, 0, WHITE);
    }
    else {
        ClearBackground(SKYBLUE);
    }
}

Background::~Background() noexcept {
    if (backgroundTexture.id > 0) {
        UnloadTexture(backgroundTexture);
    }
}