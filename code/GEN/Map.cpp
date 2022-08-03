#include "Map.h"

#include "../game.h"
#include "../ECS/Components.h"
#include "../GUI/TEXT/TextManager.h"
#include <iostream>

/* This function runs on a separate thread, created by the game class */
int Map::updateMapThreadFunction(void *ptr) {

    while (Game::_running) {
        Map::handleEvents();
        Map::update();
        Map::render();
    }

    Map::clean();

    return 0;
}

void Map::handleEvents() {
    if (Game::event->type == SDL_MOUSEBUTTONDOWN) {
        if (Game::event->button.button == SDL_BUTTON_LEFT) {
            std::cout << "Left click on map detected\n";
            Entity &e = Game::manager->addEntity();
            e.addComponent<TransformComponent>(Game::mouse->getComponent<HitboxComponent>()._r.x, Game::mouse->getComponent<HitboxComponent>()._r.y, 10, 10);
            e.addComponent<SpriteComponent>("FG_BRIGHTPURPLE_BG_BLACK", TextManager::createSourceRect('e'));
            e.addGroup(0u);
            SDL_Delay(100);
        }
    }
}

void Map::update() {

}

void Map::render() {

}

void Map::clean() noexcept {
    std::cout << "Called map clean!\n";
}