#include "raylib.h"

#include <string_view>

#include "classes/StatsDisplayer.hpp"
#include "classes/Background.hpp"

const std::string_view StatsDisplayer::spritesPaths[spritesPathsSize] = {R"(./assets/bullet_icon.png)", R"(./assets/duck_icon.png)"};

StatsDisplayer::StatsDisplayer(Game& g) noexcept : game{g}
{
    auto bulletIconImg = LoadImage(spritesPaths[0].data());
    auto duckIconImg = LoadImage(spritesPaths[1].data());

    bulletIconSprite = LoadTextureFromImage(bulletIconImg);
    duckIconSprite = LoadTextureFromImage(duckIconImg);

    font = GetFontDefault();

    if (bulletIconSprite.id <= 0) {
        TraceLog(LOG_WARNING, "Couldn't load %s, a replacement shape will be used instead.", spritesPaths[0].data());
        bulletIconRenderFunction = &bulletRender_ErrorCase;
    }
    else {
        bulletIconRenderFunction = &bulletRender;
        UnloadImage(bulletIconImg);
    }

    if (duckIconSprite.id <= 0) {
        TraceLog(LOG_WARNING, "Couldn't load %s, a replacement shape will be used instead.", spritesPaths[1].data());
        duckIconRenderFunction = &duckRender_ErrorCase;
    }
    else {
        duckIconRenderFunction = &duckRender;
        UnloadImage(duckIconImg);
    }
}

void StatsDisplayer::update() noexcept {
    // I think this is not needed
}

void StatsDisplayer::render() noexcept {
    (this->*duckIconRenderFunction)();
    (this->*bulletIconRenderFunction)();
    showStats();
}

void StatsDisplayer::duckRender() noexcept {
    DrawTexture(duckIconSprite, 10, 10, WHITE);
    DrawTexture(duckIconSprite, 10, 10 + duckHeight + 10, YELLOW);
}

void StatsDisplayer::duckRender_ErrorCase() noexcept {
    DrawRectangle(10, 10, duckWidth, duckHeight, BROWN);
}

void StatsDisplayer::bulletRender() noexcept {
    DrawTexture(bulletIconSprite, Background::width - (bulletWidth + 10 + fontSize * 3 + 10), 10, Color{75, 83, 32, 255});
}

void StatsDisplayer::bulletRender_ErrorCase() noexcept {
    DrawRectangle(Background::width - (bulletWidth + 10 + fontSize * 3 + 10), 10,bulletWidth, bulletHeight, Color{75, 83, 32, 255});
}

void StatsDisplayer::showStats() noexcept {
    constexpr u16 textVerticalPosition = 10 + (duckHeight - fontSize) / 2;

    constexpr char statTemplate[] = "x%u";
    constexpr char ducksTemplate[] = "x%u/%u";

    DrawTextEx(font, TextFormat(ducksTemplate, game.getPoints(), game.getMissedDucks()),
                Vector2{20.0f + duckWidth, textVerticalPosition}, fontSize, fontSize / 10, WHITE);
    DrawTextEx(font, TextFormat(statTemplate, game.getHighScore()),
                Vector2{20.0f + duckWidth, textVerticalPosition + duckHeight}, fontSize, fontSize / 10, WHITE);
    DrawTextEx(font, TextFormat(statTemplate, game.getBullets()),
                Vector2{Background::width - (10 + fontSize * 3), textVerticalPosition}, fontSize, fontSize / 10, WHITE);
}

StatsDisplayer::~StatsDisplayer() noexcept {
    if (duckIconSprite.id > 0) {
        UnloadTexture(duckIconSprite);
    }
    if (bulletIconSprite.id > 0) {
        UnloadTexture(bulletIconSprite);
    }
}