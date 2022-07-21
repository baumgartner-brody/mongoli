#ifndef _GAME_H_
#define _GAME_H_

#include "SDL2/SDL.h"
#include <string>

class AssetManager;
class Manager;

class Game {
public:

    static SDL_Renderer *_renderer;
    static SDL_Window *_window;
    static SDL_Event _event;
    static AssetManager *_assetManager;
    static Manager *_manager;

    Game();
    ~Game();

    void init(const std::string &window_name, const unsigned int &width, const unsigned int &height, const bool &fullscreen);

    void handleEvents();

    void update();

    void draw();

    void clean();

    const bool& running() const { return this->_running; }

private:
    bool _running = false;
};

#endif /* _GAME_H_ */