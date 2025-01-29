#ifndef DUCK_HPP_HNSS
#define DUCK_HPP_HNSS

#include "raylib.h"

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Background.hpp"

class Duck : public IGameEntity
{
public:
    // So it can still be used by the private members down there
    static constexpr u16 width = 128U;
    static constexpr u16 height = 128U;

private:
    // Wanted to use this type raw (and have stuff like void(Duck::*movementFunctions[whatever])())
    // but I decided to have mercy with my readers, so I did this nice, readable and easy to use typedef.
    using MovementFuncT = void(Duck::*)();

    static constexpr size_t spritesSize = 4;
    Texture sprites[spritesSize];
    size_t selectedSprite;
    bool textureLoadError;
    void(Duck::*selectedRenderFunction)();

    Vector2 position;
    Vector2 initialPosition; // used for calculations
    instant lastMove;
    instant lastFlap;
    bool alive;

    static constexpr millis timeBetweenSprites{ 200 };

    enum Origin : i32 {
        LEFT_SIDE = (0 - width), // so it starts outside of the screen
        RIGHT_SIDE = Background::width
    };
    static constexpr Origin originTable[2] = {Origin::LEFT_SIDE, Origin::RIGHT_SIDE};

    Origin origin;

    static constexpr size_t noMovements = 3;
    MovementFuncT movementFunctions[noMovements];
    MovementFuncT selectedMovement;

public:
    Duck() noexcept;
    void render() noexcept override;
    void update() noexcept override;
    ~Duck() noexcept override;
    void kill() noexcept;
    bool isAlive() const noexcept;
    Vector2 getPosition() const noexcept;

    static const std::string_view texturePaths[spritesSize]; // the first two are the flying sprites, the last two (coming soon) are the dying sprites

private:
    void movementFunctionConstant() noexcept;
    void movementFunctionSine() noexcept;
    void movementFunctionParabolic() noexcept;
    void movementFunctionDeath() noexcept;
    void duckRender() noexcept;
    void duckRender_ErrorCase() noexcept;

};

#endif
