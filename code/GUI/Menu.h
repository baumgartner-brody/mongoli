#ifndef _MENU_H_
#define _MENU_H_

#include "SDL2/SDL.h"

class Entity;
class HitboxComponent;

class Menu {
public:
    Menu(Entity *mMouse);
    ~Menu();

    void update();

    void draw();

private:

    Entity *_mouse = nullptr;

    HitboxComponent *_menu_rect = nullptr;

    SDL_Color rect_color = { 127, 0, 0, 255 };
};

#endif /* _MENU_H_ */