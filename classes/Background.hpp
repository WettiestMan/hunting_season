#ifndef BACKGROUND_HPP_HNSS
#define BACKGROUND_HPP_HNSS

#include "raylib.h"
#include <string_view>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"

class Background : public IGameEntity {
private:
    Texture backgroundTexture;
    Texture foregroundTexture;
    Rectangle fieldSize;

public:
    Background() noexcept;
    void update() noexcept override;
    void render() noexcept override;
    ~Background() noexcept override;

    static constexpr u16 width = 1280U;
    static constexpr u16 height = 720U;

    // shooting area bounds
    static constexpr u16 shootingAreaWidth = width;
    static constexpr u16 shootingAreaHeight = (720U - 280U);

    static const std::string_view texturePath;
};

#endif