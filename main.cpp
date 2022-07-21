#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include <iostream>

#include "Game.h"

/* Note: Regulating FPS may cause lag when closing the game window. */
/* This may be removed later in favor of better FPS methods. (FPS MAX) */
#define _FPS_DESIRED_ 120U

int main(int argv, char** args) {

	Game *game = new Game();
	game->init("WINDOW NAME", 800, 600, false);

	Uint32 FRAME_START, FRAME_TIME;
	const unsigned int FRAME_DELAY = 1000U / _FPS_DESIRED_;

	while (game->running()) {

		//FRAME_START = SDL_GetTicks();

		game->handleEvents();
		game->update();
		game->draw();

		//FRAME_TIME = SDL_GetTicks() - FRAME_START;

		//if (FRAME_DELAY > FRAME_TIME) SDL_Delay(FRAME_DELAY - FRAME_TIME);
	}

	delete game;

	return 0;
}