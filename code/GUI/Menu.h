#ifndef _MENU_H_
#define _MENU_H_

#include "SDL2/SDL.h"

#include <vector>

class Entity;
class HitboxComponent;

class Menu {
public:
    Menu();
    ~Menu();

    void init();

    void update();

    void draw();

    /* Closes the menu (calls destroy on each entity) */
    void close();

    void beginEnteringWorldName();

private:

    /* All of the entities that encompass the E */
    std::vector<Entity*> e_entities; 

    std::vector<Entity*> _new_game;
    std::vector<Entity*> _load_game;
    std::vector<Entity*> _exit_game;

    std::vector<Entity*> _enter_world_name;

    bool _entering_world_name = false;

    int _selected_option = 0;

    void _generateE() noexcept;

    void _generateOptions() noexcept;

    void _clearOptions() noexcept;

    void _TESTFUNCTION_DRAW_INN_ROOM1() noexcept;

};

#endif /* _MENU_H_ */