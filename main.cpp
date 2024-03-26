#include "include/Game.h"

#define WINDOW_WIDTH    1008
#define WINDOW_HEIGHT   753

int main()
{
    Game game = Game("Love Me Love Me Not", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT, false);
    game.Play();
    return 0;
}
