#include "game.h"

#include "ECS/Components.h"
#include "render/TextureManager.h"
#include "render/AssetManager.h"

#include "ANSI/ANSI.h"
#include "GUI/TextManager.h"

#include <iostream>

Game::Game() {

}

Game::~Game() {
    this->clean();
}

/* Make sure to declare all static game variables here  */
SDL_Renderer* Game::renderer;
Manager* Game::manager = new Manager();
AssetManager* Game::assetManager = new AssetManager();

Entity &e(Game::manager->addEntity());
Entity *e_copy;
Entity &e2(Game::manager->addEntity());
Entity &e3(Game::manager->addEntity());
Entity &e4(Game::manager->addEntity());

bool did_the_thing = false;

enum groupLabel : std::size_t {
    groupPlayers
};

/* Creates the game window and inits all necessary objects */
/*  If successful, the game runs, if not, a warning is printed and the program exits */
void Game::init(const std::string &title, const int &xpos, const int &ypos, const unsigned int &width, const unsigned int &height, const bool &fullscreen) {
    
    if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
        std::cout << "Successfully initialized SDL\n";

        /* Creates the SDL Window, uses ternary to convert the fullscreen bool to a Uint32 */
        this->window = SDL_CreateWindow(title.c_str(), xpos, ypos, width, height, (fullscreen ? SDL_WINDOW_FULLSCREEN : 0));

        if (!this->window) {
            std::cerr << "Could not create SDL Window\n";
            exit(-1);
        }

        /* Create the SDL Renderer using the window */
        Game::renderer = SDL_CreateRenderer(this->window, -1, 0);

        if (!Game::renderer) {
            std::cerr << "Could not create SDL Renderer\n";
            exit(-1);
        }

        /* Set render color to black */
        SDL_SetRenderDrawColor(Game::renderer, 0, 0, 0, 255);

        /* Initialize the SDL_Event* */
        this->event = new SDL_Event(); 

        /* Just for shits */
        std::cout << "Calling TextManager::init()\n";
        TextManager::init();

        std::vector<SDL_Color> c_in, c_out;
        c_in.emplace_back(ANSI::SDLCOLOR::BLACK);
        c_in.emplace_back(ANSI::SDLCOLOR::WHITE);
        c_out.emplace_back(ANSI::SDLCOLOR::DARK_RED);
        c_out.emplace_back(ANSI::SDLCOLOR::DARK_BLUE);

        e.addComponent<TransformComponent>(300, 300, 10, 10);
        e.addComponent<SpriteComponent>(Game::assetManager->getAsset("FG_DARKBLUE_BG_BLACK"), 10, 0, 10, 10);
        e.addGroup(groupPlayers);

        ANSI::terminalTest(std::cout);

        /* Attempting to get a subsurface */
        SDL_Surface *s(TextureManager::SurfaceTools::createSurface(FONT_TEXTURE_FILE));
        SDL_Surface *sub(TextureManager::SurfaceTools::createSubSurface(s, TextManager::createSourceRect('e')));
        SDL_FreeSurface(s);
        Game::assetManager->addAsset("TEST", TextureManager::TextureTools::createTexture(sub));

        e4.addComponent<TransformComponent>(100, 100, 10, 10);
        SDL_Rect srcR = { 0, 0, 10, 10 };
        e4.addComponent<SpriteComponent>("TEST", srcR);
        e4.addGroup(groupPlayers);

        e_copy = &Game::manager->copyEntity(e);

        e2.addComponent<TransformComponent>(400, 300, 10, 10);
        e2.addComponent<SpriteComponent>(Game::assetManager->getAsset("FG_BRIGHTPURPLE_BG_BLACK"), 10, 0, 10, 10);
        e2.addGroup(groupPlayers);

        e3.addComponent<TransformComponent>(300, 400, 20, 20);
        e3.addComponent<SpriteComponent>("FG_BRIGHTGREEN_BG_BLACK", TextManager::createSourceRect(127));
        e3.addGroup(groupPlayers);

        /* Tell the game it is safe to update */
        this->_running = true;

    } else {
        std::cerr << "Could not initialize SDL\n";
        exit(-1);
    }
}

void Game::handleEvents() {
    SDL_PollEvent(this->event);
    switch (this->event->type) {
        case SDL_QUIT:
            this->_running = false;
            break;
        default:
            break;
    }
}

/* Updates all game objects */
void Game::update() {

    /* Refresh the manager and delete any unActive entities and then update() the remaining ones */
    Game::manager->refresh();
    Game::manager->update();

    if (SDL_GetTicks() >= 6000 && !did_the_thing) {
        e_copy->getComponent<TransformComponent>().xpos = 200;
        std::cout << "moved e_copy over\n";
        did_the_thing = true;
    }
}

/* Presents the game to an SDL Renderer */
void Game::render() {

    auto &players(Game::manager->getGroup(groupPlayers));

    SDL_RenderClear(Game::renderer);

    for (auto & p : players) p->draw();

    SDL_RenderPresent(Game::renderer);
}

/* Cleans all heap resources owned by the Game object */
void Game::clean() {
    /* Free the window */
    SDL_DestroyWindow(this->window);
    this->window = nullptr;

    /* Free the renderer */
    SDL_DestroyRenderer(Game::renderer);
    Game::renderer = nullptr;

    SDL_Quit();

    /* Free the SDL_Event */
    delete this->event;
    this->event = nullptr;

    /* Free/clear the manager */
    delete Game::manager;
    Game::manager = nullptr;

    /* Free/clear the assetmanager */
    delete Game::assetManager;
    Game::assetManager = nullptr;

    std::cout << "Successfully cleaned Game object\n";
}