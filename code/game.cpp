#include "game.h"

#include "ECS/Components.h"
#include "render/TextureManager.h"
#include "render/AssetManager.h"

#include "PHYSICS/Collision.h"

#include "ANSI/ANSI.h"
#include "GUI/TEXT/TextManager.h"
#include "GUI/Menu.h"

#include "GEN/Worldgen.h"
#include "GEN/Map.h"

#include <iostream>
#include <sstream> // mouse position = window title 

Game::Game() {

}

Game::~Game() {
    this->clean();
}

/* Make sure to declare all static game variables here  */
SDL_Renderer* Game::renderer;
Manager* Game::manager = new Manager();
AssetManager* Game::assetManager = new AssetManager();
Entity* Game::mouse = &(Game::manager->addEntity());
KeyboardController* Game::keyboard;
bool Game::_running = false;

/* Initialize the SDL_Event* */
SDL_Event* Game::event = new SDL_Event(); 
SDL_Thread *mapThread;

/*
Entity &e(Game::manager->addEntity());
Entity *e_copy;
Entity &e2(Game::manager->addEntity());
Entity &e3(Game::manager->addEntity());
Entity &e4(Game::manager->addEntity());
Entity &e5(Game::manager->addEntity());
*/

Menu* menu = new Menu();

/* Sound test 7/22/2022 */
Mix_Chunk *mix_chunk = nullptr;

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

        /* Just for shits */
        std::cout << "Calling TextManager::init()\n";
        TextManager::init();

        TextManager::TEXT_POSITION_X = 300;

        menu->init();

        std::vector<SDL_Color> c_in, c_out;
        c_in.emplace_back(ANSI::SDLCOLOR::BLACK);
        c_in.emplace_back(ANSI::SDLCOLOR::WHITE);
        c_out.emplace_back(ANSI::SDLCOLOR::DARK_RED);
        c_out.emplace_back(ANSI::SDLCOLOR::DARK_BLUE);

        /*
        e.addComponent<TransformComponent>(300, 300, 10, 10);
        e.addComponent<SpriteComponent>(Game::assetManager->getAsset("FG_DARKBLUE_BG_BLACK"), 10, 0, 10, 10);
        e.addGroup(groupPlayers);
        */

        ANSI::terminalTest(std::cout);

        /* Attempting to get a subsurface */
        SDL_Surface *s(TextureManager::SurfaceTools::createSurface(FONT_TEXTURE_FILE));
        SDL_Surface *sub(TextureManager::SurfaceTools::createSubSurface(s, TextManager::createSourceRect('e')));
        SDL_FreeSurface(s);
        Game::assetManager->addAsset("TEST", TextureManager::TextureTools::createTexture(sub));

        /*
        e4.addComponent<TransformComponent>(100, 100, 10, 10);
        SDL_Rect srcR = { 0, 0, 10, 10 };
        e4.addComponent<SpriteComponent>("TEST", srcR);
        e4.addGroup(groupPlayers);

        e_copy = &Game::manager->copyEntity(e);

        e2.addComponent<TransformComponent>(400, 300, 10, 10);
        e2.addComponent<SpriteComponent>("FG_BRIGHTPURPLE_BG_BLACK", 10, 0, 10, 10);
        e2.addComponent<HitboxComponent>(0, 0, 10, 10); // TODO: Fix Hitbox interface
        e2.addComponent<KeyboardController>();
        e2.addGroup(groupPlayers);
        */

        /* Set the Game's universal keyboardcontroller* to the player's keyboard */
        //Game::keyboard = &e2.getComponent<KeyboardController>();

        /*
        e3.addComponent<TransformComponent>(300, 400, 20, 20);
        e3.addComponent<SpriteComponent>("FG_BRIGHTGREEN_BG_BLACK", TextManager::createSourceRect(127));
        e3.addComponent<HitboxComponent>(0, 0, 20, 20);
        e3.addGroup(groupPlayers);
        */

        /*
        e5.addComponent<TransformComponent>(500, 500, 10, 10);
        e5.addComponent<SpriteComponent>("FG_BRIGHTRED_BG_BLACK", 0, 0, 10, 10);
        e5.getComponent<SpriteComponent>().addAnimation(0, 4, 1000, 40);
        e5.getComponent<SpriteComponent>().play(0);
        e5.addGroup(groupPlayers);
        */

       WorldGen::test();

        mouse->addComponent<HitboxComponent>(0, 0, 10, 10);
        mouse->addComponent<MouseComponent>();
        //mouse.addGroup(groupPlayers);

        /* Initialize the sound mixer */
        if (Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1) {
            std::cerr << "Could not initialize audio!\n";
            exit(-1);
        }

        mix_chunk = Mix_LoadWAV("src/assets/high-pitch-sound.wav");

        if (mix_chunk == nullptr) {
            std::cerr << "The soundfile could not be loaded!\n";
            std::cerr << "Error: \"" << SDL_GetError() << "\"\n";
            exit(-1);
        }

        //TextManager::addText("FG_BRIGHTRED_BG_BLACK", "Hello");

        /* Tell the game it is safe to update */
        this->_running = true;

    } else {
        std::cerr << "Could not initialize SDL\n";
        exit(-1);
    }

    mapThread = SDL_CreateThread(Map::updateMapThreadFunction, "MapThread", (void*)NULL);
    SDL_DetachThread(mapThread);
}

void Game::handleEvents() {
    SDL_PollEvent(this->event);

    /* Key.repeat seems to be more smooth than manually creating bools */
    if (this->event->type == SDL_QUIT) {
        this->_running = false;
    } else if (!this->event->key.repeat && this->event->type == SDL_KEYDOWN) {
        if (this->event->key.keysym.sym == SDLK_1) {
            
        } else if (this->event->key.keysym.sym == SDLK_2) {
            //_2 = true;

            /* Report that 2 was pressed and play the test sound. */
            std::cout << "2 was pressed.\n";
            if ( Mix_PlayChannel(-1, mix_chunk, 0) == -1 ) {
                std::cerr << "That audio could not be played.\n";
                exit(-1);
            }
        }
    } else if (this->event->type == SDL_KEYUP) {
        if (this->event->key.keysym.sym == SDLK_2) {
            //_2 = false;

            /* Report that 2 was pressed and play the test sound. */
            std::cout << "2 was released.\n";
            Mix_HaltChannel(-1);
        }
    }

    /*
    switch (this->event->type) {
        case SDL_KEYDOWN:
            switch (this->event->key.keysym.sym) {
                case SDLK_2:
                    if (!_2) {
                    std::cout << "2 was pressed.\n";
                    if ( Mix_PlayChannel(-1, mix_chunk, 0) == -1 ) {
                        std::cerr << "That audio could not be played.\n";
                        exit(-1);
                    }
                    break;
                    }
                default:
                    break;
            }
            break;
        case SDL_KEYUP:
            switch (this->event->key.keysym.sym) {
                case SDLK_2:
                    _2 = false;
                    std::cout << "2 was released.\n";
                    if ( Mix_HaltChannel(-1) == -1 ) {
                        std::cerr << "The audio could not be halted.\n";
                        exit(-1);
                    }
                    break;
                default:
                    break;
            }
            break;
        case SDL_QUIT:
            this->_running = false;
            break;
        default:
            break;
    }
    */
}

/* Updates all game objects */
void Game::update() {

    /* Refresh the manager and delete any unActive entities and then update() the remaining ones */
    Game::manager->refresh();
    Game::manager->update();

    menu->update();

    std::stringstream ss;
    ss << "X: " << mouse->getComponent<HitboxComponent>()._r.x << " Y: " << mouse->getComponent<HitboxComponent>()._r.y;

    SDL_SetWindowTitle(Game::window, ss.str().c_str());
}

/* Presents the game to an SDL Renderer */
void Game::render() {

    auto &players(Game::manager->getGroup(groupPlayers));

    SDL_RenderClear(Game::renderer);

    Map::render();

    for (auto & p : players) p->draw();

    menu->draw();

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
    delete Game::event;
    Game::event = nullptr;

    /* Free the menu */
    delete menu;
    menu = nullptr;

    /* Free/clear the manager */
    delete Game::manager;
    Game::manager = nullptr;

    /* Free the mouse + keyboard (done automatically by the manager) */
    Game::mouse = nullptr;
    Game::keyboard = nullptr;

    /* Free/clear the assetmanager */
    delete Game::assetManager;
    Game::assetManager = nullptr;

    TextManager::free();

    /* Free sound */
    Mix_FreeChunk(mix_chunk);
    mix_chunk = nullptr;

    Mix_CloseAudio();

    std::cout << "Successfully cleaned Game object\n";
}