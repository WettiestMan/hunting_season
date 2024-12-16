#include "raylib.h"
#include <ctime>

#include "classes/Background.hpp"
#include "classes/Crosshair.hpp"

int main(int argc, char** argv) {
    SetRandomSeed(std::time(nullptr));
    InitWindow(Background::width, Background::height, "XD");

    Background bg;
    Crosshair ch;
    
    SetTargetFPS(60);
    TraceLog(LOG_INFO, TextFormat("Program current path is %s", GetWorkingDirectory()));
    while (!WindowShouldClose()) {
        BeginDrawing();
        bg.update();
        bg.render();
        ch.update();
        ch.render();
        EndDrawing();
    }
    CloseWindow();

    return 0;
}
