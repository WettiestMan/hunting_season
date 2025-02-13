#ifndef BULLETPACK_HPP_HNSS
#define BULLETPACK_HPP_HNSS

#include "raylib.h"

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"

class BulletPack : public IGameEntity {
public:
    static constexpr u16 width = 128U;
    static constexpr u16 height = 128U;

private:
    static constexpr size_t spritesSize = 4;
    const Texture* spritesBufferPtr;
    size_t selectedSprite;
    bool textureLoadError;
    void(BulletPack::*selectedRenderFunction)();

    Vector2 position;
    Vector2 initialPosition;
    i32 PositionPeakY;
    instant lastMove;
    instant lastSpriteSwitch;
    bool alive;

    static constexpr millis timeBetweenSprites{ 250 };

public:
    BulletPack(const Texture* txs) noexcept;
    void update() noexcept override;
    void render() noexcept override;
    ~BulletPack() noexcept override;

    void kill() noexcept;
    bool isAlive() const noexcept;
    Vector2 getPosition() const noexcept;
};

#endif