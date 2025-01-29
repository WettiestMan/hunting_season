#ifndef CROSSHAIR_CPP_HNSS
#define CROSSHAIR_CPP_HNSS

#include "raylib.h"
#include <string_view>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Game.hpp"

class Crosshair : public IGameEntity {
private:
    Game& game;

    static constexpr size_t framesSize = 2;
    Texture blasts[framesSize];
    Texture cross;

    Vector2 position;
    u16 width;
    u16 height;
    Vector2 lastPressedPosition;

    bool shotTriggered;
    instant shotTriggeredInstant;
    u8 blastSelectedTexture;

    bool blastLoadError;

    static constexpr size_t blastRenderRoutinesSize = 2;

    void(Crosshair::*blastRenderRoutines[blastRenderRoutinesSize])();
    
    static constexpr u16 okWidth = 32U;
    static constexpr u16 okHeight = 32U;
    static constexpr u16 backupWidth = 8U;
    static constexpr u16 backupHeight = 8U;
    static constexpr u16 blastWidth = 48U;
    static constexpr u16 blastHeight = 48U;
    static constexpr millis blastFrameDisplayTime { 500 };

public:
    Crosshair(Game& g) noexcept;
    virtual void update() noexcept override;
    virtual void render() noexcept override;
    virtual ~Crosshair() noexcept override;

    static const std::string_view texturePath;
    static const std::string_view blast1TexturePath;
    static const std::string_view blast2TexturePath;

private:
    void blastRender() noexcept;
    void blastRender_ErrorCase() noexcept;
};

#endif