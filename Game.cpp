#include "Game.h"
#include "RENDER/AssetManager.h"
#include "RENDER/TextureManager.h"
#include "ECS/Components.h"

#include <iostream>
#include <vector>

SDL_Renderer *Game::_renderer = nullptr;
SDL_Window *Game::_window = nullptr;
SDL_Event Game::_event;
AssetManager *Game::_assetManager = new AssetManager;
Manager *Game::_manager = new Manager;

SDL_Rect tstR = { 100, 100, 50, 50 };
SDL_Rect tstR2 = { 200, 200, 50, 50 };

Entity &e(Game::_manager->addEntity());
Entity &e2(Game::_manager->addEntity());

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

    /* Test recolor function green -> black */
    SDL_Color green = { 0, 255, 0, 255 };
    SDL_Color black = { 0, 0, 0, 255 };
    SDL_Color white = { 255, 255, 255, 255 };
    SDL_Color yellow = { 255, 255, 0, 255 };

    TextureManager::Surface::recolorSurface(s, green, black);

    std::vector<SDL_Color> c_in = { black, white };
    std::vector<SDL_Color> c_out = { white, green };

    TextureManager::Surface::recolorSurface(s2, c_in, c_out);

    SDL_Texture *t, *t2;

    e.addComponent<Transform>(100, 100, 50, 50);

    std::cout << "e == e2 ? " << (e == e2) << '\n';

    e2 = e;

    std::cout << "e == e2 ? " << (e == e2) << '\n';

    t = TextureManager::Texture::createTexture(s);
    t2 = TextureManager::Texture::createTexture(s2);

    Game::_assetManager->getAsset("TEXTURE_1", t);
    Game::_assetManager->getAsset("TEXTURE_2", t2);

    e.addComponent<Sprite>("TEXTURE_2");
    e2.getComponent<Transform>()._r.x = 200;
    e2.getComponent<Transform>()._r.y = 200;
    e2.addComponent<Sprite>("TEXTURE_1");

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
    
    Game::_manager->refresh();
    Game::_manager->update();
}

void Game::draw() {
    SDL_RenderClear(this->_renderer);

    e.draw();
    e2.draw();

    SDL_RenderPresent(this->_renderer);
}

void Game::clean() {
    std::cout << "Called Game::clean()\n";
    SDL_DestroyRenderer(this->_renderer);
	SDL_DestroyWindow(this->_window);
	SDL_Quit();
    
    delete Game::_assetManager;
    Game::_assetManager = nullptr;

    delete Game::_manager;
    Game::_manager = nullptr;

    std::cout << "Cleaned Game Object!\n";
}