#include <span>

#include "classes/Game.hpp"

int main(int argc, char** argv) {
    Game game(std::span<char*>(argv, argc));

    return 0;
}
