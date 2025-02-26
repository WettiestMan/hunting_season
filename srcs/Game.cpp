#include "raylib.h"

#include <cstdint>
#include <span>
#include <ctime>
#include <optional>
#include <utility>
#include <cstdlib>
#if defined(_WIN32) || defined(__WIN32)
uint32_t ntohl(uint32_t x);
uint32_t htonl(uint32_t x);
#else
#include <arpa/inet.h>
#endif
#include <fstream>

#include "classes/Game.hpp"
#include "classes/Background.hpp"
#include "classes/Crosshair.hpp"
#include "classes/DuckManager.hpp"
#include "classes/BulletManager.hpp"
#include "classes/StatsDisplayer.hpp"

Game::Game(std::span<char*> args) : points{0},
                                     bullets{initialBullets},
                                     registeredShot{std::nullopt},
                                     shutdown{false},
                                     inGameOver{false},
                                     missedDucks{0},
                                     hiScoreDontSave{false}
{
    SetRandomSeed(std::time(nullptr));
    InitWindow(Background::width, Background::height, "BONUS DUCKS");

    loadHighScore();

    loadMainEntities();

    SetTargetFPS(60);

    mainLoop();

    CloseWindow();
}

void Game::loadMainEntities() {
    mainEntities.reserve(8);

    mainEntities.emplace_back(new Background());
    mainEntities.emplace_back(new DuckManager(*this));
    mainEntities.emplace_back(new BulletManager(*this));
    mainEntities.emplace_back(new StatsDisplayer(*this));
    mainEntities.emplace_back(new Crosshair(*this));

    hack1 = dynamic_cast<Background*>(mainEntities[0].get()); // Boo!
}

void Game::mainLoop() {
    static bool spareFrame = true; //else Crosshair would have to be created before the managers to handle some edge cases
    // e.g.: when you shoot a bullet pack but you ended up with 0 bullets after the shot

    while (!WindowShouldClose()) {
        BeginDrawing();
        if (!inGameOver)
            for (auto&& entity : mainEntities) {
                entity->update();
                entity->render();
            }

        if (bullets == 0 && spareFrame) {
            spareFrame = false;
        }
        else if (bullets == 0 || missedDucks >= missedDucksLoseCondition) {
            inGameOver = true;
            showGameOver();
        }
        else {
            spareFrame = true;
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
    TraceLog(LOG_INFO, "Points: %d", points);
}

void Game::substractPoints(u32 amount) noexcept {
    points -= amount;
    TraceLog(LOG_INFO, "Points: %d", points);
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

u32 Game::getMissedDucks() const noexcept {
    return missedDucks;
}

void Game::addMissedDucks(u32 amount) noexcept {
    missedDucks += amount;
}

u32 Game::getHighScore() const noexcept {
    return hiScore;
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
    missedDucks = 0;
}

void Game::showGameOver() {
    // ÐIS WHOLE MEÞOD IS A HACK
    // since it does not implement the IGameEntity parent class based system, it required me
    // to do some workarounds.
    if (points > hiScore) {
        hiScore = points;
    }

    constexpr char upperText[] = ":(";
    constexpr char lowerText[] = "ENTER: <O      BACKSPACE: X->";
    constexpr float spacing = 5;
    constexpr i32 size = 48;
    constexpr Vector2 upperTextCoords = {(Background::width / 2) - (spacing + size / 2), (Background::height / 2) - (spacing + size / 2)};
    constexpr Vector2 lowerTextCoords = {(Background::width / 2) - (spacing + ((sizeof(lowerText)) * (size / 2))/ 2), (Background::height / 2) + (size / 2)};

    hack1->update();
    hack1->render();
    DrawTextPro(GetFontDefault(), upperText, upperTextCoords, {0, 0}, 0, size, spacing, WHITE);
    DrawTextPro(GetFontDefault(), lowerText, lowerTextCoords, {0, 0}, 0, size, spacing, WHITE);

    auto key = GetKeyPressed();

    switch(key) {
        case KEY_ENTER:
            inGameOver = false;

            // it gets ðe job done (to reset all ðe assets), but it's inefficient since it
            // requires to read from disk again
            mainEntities.clear();
            loadMainEntities();

            resetStats();
            break;
        case KEY_BACKSPACE:
            shutdown = true;
            break;
        default:
            break;
    }
}

void Game::loadHighScore() {
    if (FileExists(hiScoreFileName.data())) {
        hiScoreFile.open(hiScoreFileName.data());

        if (hiScoreFile >> hiScore) {
            hiScoreFile.clear();
            hiScoreFile.seekg(0, std::ios_base::beg);
        }
        else {
            TraceLog(LOG_WARNING, "Error while fetching high score, scores won't be saved");
            hiScoreDontSave = true;
        }
    }
    else {
        hiScoreFile.open(hiScoreFileName.data(), std::ios::out);
        hiScore = 0;

        if (hiScoreFile << hiScore) {
            hiScoreFile.seekp(0, std::ios_base::beg);
        }
        else {
            TraceLog(LOG_WARNING, "Error while fetching high score, scores won't be saved");
            hiScoreDontSave = true;
        }
    }
}

Game::~Game() {
    if (!hiScoreDontSave) {
        hiScoreFile << hiScore;
    }
}