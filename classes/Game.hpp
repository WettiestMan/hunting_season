#ifndef GAME_HPP_HNSS
#define GAME_HPP_HNSS

#include "raylib.h"
#include <vector>
#include <memory>
#include <optional>
#include <span>
#include <string_view>
#include <fstream>

#include "base/IGameEntity.hpp"
#include "defs/TypeAliasses.hpp"
#include "Background.hpp"

class Game {
private:
    static constexpr std::string_view hiScoreFileName = "hiscore.dat";
    std::fstream hiScoreFile;

    std::vector<std::unique_ptr<IGameEntity>> mainEntities;
    u32 points;
    u32 bullets;
    u32 hiScore;
    u32 missedDucks;

    bool inGameOver;
    bool hiScoreDontSave;
    bool shutdown;

    std::optional<Vector2> registeredShot;

    static constexpr u32 initialBullets = 6;
    static constexpr u32 missedDucksLoseCondition = 3;

    Background* hack1; // a hack to display the background during game over

public:
    Game(std::span<char*> args);

    u32 getPoints() const noexcept;
    void addPoints(u32 amount) noexcept;
    void substractPoints(u32 amount) noexcept;

    u32 getBullets() const noexcept;
    void addBullets(u32 amount) noexcept;
    void substractBullets(u32 amount) noexcept;
    void resetBullets() noexcept;

    u32 getMissedDucks() const noexcept;
    void addMissedDucks(u32 amount) noexcept;

    u32 getHighScore() const noexcept;

    void showGameOver();

    void resetStats() noexcept;

    const std::optional<Vector2>& getRegisteredShot() const noexcept;
    void setRegisteredShot(std::optional<Vector2> toRegister) noexcept;

    ~Game();

private:
    void loadMainEntities();
    void mainLoop();
    void loadHighScore();
};

#endif