#include "game.h"

/* In the final this is going to be a setting for the user */
#define DESIRED_FPS 60

/* SDL Requires main to take in these params in order to override it. */
int main(int argc, char **argv) {
    
    /* Create a singular game object */
    Game *game = new Game();
    game->init("Window Title", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 640, false);

    const unsigned int FRAME_DELAY = (1000 / DESIRED_FPS);

    /* The Uint32 timestamp of the current frame's start time */
    Uint32 FRAME_START;

    /* The amount of time the current frame took in ms */
    unsigned int FRAME_TIME;

    while (Game::_running) {

        /* Get elapsed ms from the beginning of SDL_Init */
        FRAME_START = SDL_GetTicks();

        game->handleEvents();
        game->update();
        game->render();

        /* Use FRAME_START and current time to determine how long this frame took */
        FRAME_TIME = SDL_GetTicks() - FRAME_START;

        /* Regulate framerate */
        if (FRAME_DELAY > FRAME_TIME) SDL_Delay(FRAME_DELAY - FRAME_TIME);
    }

    game->clean();

    return 0;
}