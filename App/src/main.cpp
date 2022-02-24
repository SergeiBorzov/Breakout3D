#include "game.h"

int main() {
    Game game;
    game.Init("Circular breakout", 1920, 1080, true);
    game.Run();
    return 0;
}
