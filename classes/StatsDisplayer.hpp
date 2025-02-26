#ifndef STATS_DISPLAYER_HPP_HNSS
#define STATS_DISPLAYER_HPP_HNSS

#include "raylib.h"

#include <string_view>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Game.hpp"

class StatsDisplayer : public IGameEntity {
private:
    static constexpr size_t spritesPathsSize = 2;
    static constexpr u16 duckWidth = 64;
    static constexpr u16 duckHeight = 64;
    static constexpr u16 bulletWidth = 64;
    static constexpr u16 bulletHeight = 64;

    Game& game;
    Texture duckIconSprite;
    Texture bulletIconSprite;
    static const std::string_view spritesPaths[spritesPathsSize];
    Font font;
    static constexpr u8 fontSize = 36;

    void(StatsDisplayer::*bulletIconRenderFunction)();
    void(StatsDisplayer::*duckIconRenderFunction)();
    
public:
    StatsDisplayer(Game& g) noexcept;
    void update() noexcept override;
    void render() noexcept override;
    ~StatsDisplayer() noexcept override;

private:
    void bulletRender() noexcept;
    void duckRender() noexcept;
    void bulletRender_ErrorCase() noexcept;
    void duckRender_ErrorCase() noexcept;
    void showStats() noexcept;
};

#endif