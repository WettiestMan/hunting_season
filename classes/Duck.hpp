#ifndef DUCK_HPP_HNSS
#define DUCK_HPP_HNSS

#include <cstdint>
#include "raylib.h"
#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"

class Duck : public IGameEntity
{
private:
    Texture frames[4];
    Vector2 position;

    static constexpr size_t noFunctionPaths = 3;
    void(Duck::*functionPaths[noFunctionPaths])();

public:
    Duck() noexcept;
    Duck(u16 ix, u16 iy) noexcept;
    virtual void render() noexcept override;
    virtual void update() noexcept override;
    virtual ~Duck() noexcept override;

private:
    void functionPathLinear() noexcept;
    void functionPathSine() noexcept;
    void functionPathExp() noexcept; 
};

#endif
