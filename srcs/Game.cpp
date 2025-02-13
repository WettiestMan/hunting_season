#include "raylib.h"

#include <span>
#include <ctime>
#include <optional>
#include <utility>
#include <cstdlib>

#include "classes/Game.hpp"
#include "classes/Background.hpp"
#include "classes/Crosshair.hpp"
#include "classes/DuckManager.hpp"
#include "classes/Duck.hpp"

Game::Game(std::span<char*> args) : points{0},
                                     bullets{initialBullets},
                                     registeredShot{std::nullopt},
                                     shutdown{false}
{
    SetRandomSeed(std::time(nullptr));
    InitWindow(Background::width, Background::height, "BONUS DUCKS");

    loadMainEntities();

    SetTargetFPS(60);

    mainLoop();

    CloseWindow();
}

void Game::loadMainEntities() {
    mainEntities.reserve(8);

    mainEntities.emplace_back(new Background());
    mainEntities.emplace_back(new Crosshair(*this));
    mainEntities.emplace_back(new DuckManager(*this));
}

void Game::mainLoop() {
    while (!WindowShouldClose()) {
        BeginDrawing();
        for (auto&& entity : mainEntities) {
            entity->update();
            entity->render();
        }

        if (bullets == 0) {
            showGameOver();
        }
        EndDrawing();
        if (shutdown)
            break;
    }
}

u32 Game::getPoints() const noexcept {
    return points;
}

void Game::addPoints(u32 amount) noexcept {
    points += amount;
    #ifndef DONT_SHOW_DETAILS
    TraceLog(LOG_INFO, "Points: %d", points);
    #endif
}

void Game::substractPoints(u32 amount) noexcept {
    points -= amount;
    #ifndef DONT_SHOW_DETAILS
    TraceLog(LOG_INFO, "Points: %d", points);
    #endif
}

u32 Game::getBullets() const noexcept {
    return bullets;
}

void Game::addBullets(u32 amount) noexcept {
    bullets += amount;
    TraceLog(LOG_INFO, "bullets: %d", bullets);
}

void Game::substractBullets(u32 amount) noexcept {
    bullets -= amount;
    TraceLog(LOG_INFO, "Bullets: %d", bullets);
}

void Game::resetBullets() noexcept {
    bullets = initialBullets;
    TraceLog(LOG_INFO, "Bullets: %d", bullets);
}

const std::optional<Vector2>& Game::getRegisteredShot() const noexcept {
    return registeredShot;
}

void Game::setRegisteredShot(std::optional<Vector2> toRegister) noexcept {
    registeredShot = toRegister;
}

void Game::resetStats() noexcept {
    points = 0;
    bullets = initialBullets;
}

void Game::showGameOver() {
    shutdown = true;
}