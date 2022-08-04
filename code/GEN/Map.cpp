#include "Map.h"

#include "../game.h"
#include "../ECS/Components.h"
#include "../GUI/TEXT/TextManager.h"
#include <iostream>

Manager *Map::_manager;

/* This function runs on a separate thread, created by the game class */
int Map::updateMapThreadFunction(void *ptr) {

    if (Map::init() != 0) return -1;

    while (Game::_running) {
        Map::handleEvents();
        Map::update();
        //Map::render();
    }

    Map::clean();

    return 0;
}

int Map::init() {

    Map::_manager = new Manager;

    return (Map::_manager == NULL) ? -1 : 0;
}

void Map::handleEvents() {
    if (Game::event->type == SDL_MOUSEBUTTONDOWN) {
        if (Game::event->button.button == SDL_BUTTON_LEFT) {
            std::cout << "Left click on map detected\n";
            Entity &e = Map::_manager->addEntity();
            e.addComponent<TransformComponent>(Game::mouse->getComponent<HitboxComponent>()._r.x, Game::mouse->getComponent<HitboxComponent>()._r.y, 10, 10);
            e.addComponent<SpriteComponent>("FG_BRIGHTPURPLE_BG_BLACK", TextManager::createSourceRect('e'));
            e.addGroup(0u);
            SDL_Delay(100);
        }
    }
}

void Map::update() {

    Map::_manager->refresh();
    Map::_manager->update();

}

void Map::render() {
    for (auto & e : Map::_manager->getGroup(0u)) e->draw();
}

void Map::clean() noexcept {
    std::cout << "Called map clean!\n";

    Map::_manager->clear();

    delete Map::_manager;
    Map::_manager = nullptr;

    std::cout << "Cleaned map object!\n";
}