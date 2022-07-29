#ifndef _GAME_H_
#define _GAME_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_mixer.h"
#include <string>

class Manager;
class AssetManager;

class Game {
public:
    Game();
    ~Game();

    /* Creates the game window and inits all necessary objects */
    void init(const std::string &title, const int &xpos, const int &ypos, const unsigned int &width, const unsigned int &height, const bool &fullscreen);

    /* Handles SDL events */
    void handleEvents();

    /* Updates all game objects */
    void update();

    /* Presents the game to an SDL Renderer */
    void render();

    /* Cleans all heap resources owned by the Game object */
    void clean();

    static SDL_Event *event;

    const bool& running() const { return this->_running; }

    /* One renderer to rule them all. */
    static SDL_Renderer *renderer;

    static Manager *manager;
    static AssetManager *assetManager;

private:

    SDL_Window *window = nullptr;

    bool _running = false;
};

#endif /* _GAME_H_ */