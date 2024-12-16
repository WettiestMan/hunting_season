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

    // shooting area bounds
    static constexpr u16 shootingAreaWidth = 1280U;
    static constexpr u16 shootingAreaHeight = (720U - 280U);
public:
    Background() noexcept;
    virtual void update() noexcept override;
    virtual void render() noexcept override;
    virtual ~Background() noexcept override;

    static constexpr u16 width = shootingAreaWidth;
    static constexpr u16 height = 720U;

    static const std::string_view texturePath;
};

#endif