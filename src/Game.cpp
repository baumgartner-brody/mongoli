#include "Game.h"
#include "RENDER/TextureManager.h"

#include <iostream>

SDL_Renderer *Game::_renderer = nullptr;
SDL_Window *Game::_window = nullptr;
SDL_Event Game::_event;

/* Global texture for testing texture drawing capability */
SDL_Texture *t;

SDL_Rect tstR = { 100, 100, 50, 50 };

Game::Game() {

}

Game::~Game() {
    this->clean();
}

void Game::init(const std::string &window_name, const unsigned int &width, const unsigned int &height, const bool &fullscreen) {

    SDL_Init(SDL_INIT_EVERYTHING);
    
    this->_window = SDL_CreateWindow(window_name.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, (fullscreen) ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);

    if (!this->_window) {
        std::cerr << "There was an error creating the SDL Window\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1);
    }

	this->_renderer = SDL_CreateRenderer(this->_window, -1, 0);
    if (!this->_renderer) {
        std::cerr << "There was an error creating the SDL Renderer\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1);
    }

    SDL_SetRenderDrawColor(this->_renderer, 255, 0, 0, 255);

    SDL_Surface *s = TextureManager::createSurface("src/test.png");
    t = TextureManager::createTexture(s);

    /* Important - all surfaces and textures must be freed when their time is up */
    SDL_FreeSurface(s);

    this->_running = true;
}

void Game::handleEvents() {
    SDL_PollEvent(&this->_event);

    if (this->_event.type == SDL_QUIT)
        this->_running = false;
    
    if (this->_event.type == SDL_KEYDOWN)
        if (this->_event.key.keysym.sym == SDLK_ESCAPE)
            this->_running = false;
}

void Game::update() {
    /* stuff */
}

void Game::draw() {
    SDL_RenderClear(this->_renderer);

    TextureManager::draw(t, tstR);

    SDL_RenderPresent(this->_renderer);
}

void Game::clean() {
    SDL_DestroyTexture(t);
    SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);
	SDL_Quit();
}