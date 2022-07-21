#include "Game.h"
#include "RENDER/TextureManager.h"

#include <iostream>
#include <vector>

SDL_Renderer *Game::_renderer = nullptr;
SDL_Window *Game::_window = nullptr;
SDL_Event Game::_event;

/* Global texture for testing texture drawing capability */
SDL_Texture *t;
SDL_Texture *t2;

SDL_Rect tstR = { 100, 100, 50, 50 };
SDL_Rect tstR2 = { 200, 200, 50, 50 };

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

    SDL_Surface *s = TextureManager::Surface::createSurface("src/test.png");
    SDL_Surface *s2 = TextureManager::Surface::createSurface("src/test1bit.png");
    //SDL_Surface *s2_cpy = TextureManager::Surface::copySurface(s2);

    std::cout << "Is test.png 32 bit ? " << TextureManager::Surface::is32bit(s) << '\n';
    std::cout << "Is test1bit.png 32 bit ? " << TextureManager::Surface::is32bit(s2) << '\n';

    /* Test recolor function red -> black */
    SDL_Color red = { 255, 0, 0, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    TextureManager::Surface::recolorSurface(s, red, black);

    std::vector<SDL_Color> c_in = { black, white };
    std::vector<SDL_Color> c_out = { white, red };

    TextureManager::Surface::recolorSurface(s2, c_in, c_out);

    t = TextureManager::Texture::createTexture(s);
    t2 = TextureManager::Texture::createTexture(s2);


    SDL_FreeSurface(s);
    SDL_FreeSurface(s2);

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

    TextureManager::Texture::draw(t, tstR);
    TextureManager::Texture::draw(t2, tstR2);

    SDL_RenderPresent(this->_renderer);
}

void Game::clean() {
    std::cout << "Called Game::clean()\n";
    SDL_DestroyTexture(t);
    SDL_DestroyTexture(t2);
    SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);
	SDL_Quit();

    std::cout << "Cleaned Game Object!\n";
}