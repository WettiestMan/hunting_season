#ifndef GAME_HPP_HNSS
#define GAME_HPP_HNSS

#include "raylib.h"
#include <vector>
#include <memory>
#include <optional>
#include <span>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"

class Game {
private:
    std::vector<std::unique_ptr<IGameEntity>> mainEntities;
    u32 points;
    u32 bullets;

    std::optional<Vector2> registeredShot;

    static constexpr u32 initialBullets = 6;

public:
    Game(std::span<char*> args);

    u32 getPoints() const noexcept;
    void addPoints(u32 amount) noexcept;
    void substractPoints(u32 amount) noexcept;

    u32 getBullets() const noexcept;
    void addBullets(u32 amount) noexcept;
    void substractBullets(u32 amount) noexcept;

    [[noreturn]] void showGameOver();

    void resetStats() noexcept;

    const std::optional<Vector2>& getRegisteredShot() const noexcept;
    void setRegisteredShot(std::optional<Vector2> toRegister) noexcept;

private:
    void loadMainEntities();
    void mainLoop();
};

#endif