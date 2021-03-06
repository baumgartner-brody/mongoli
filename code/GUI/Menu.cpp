#include "Menu.h"

#include "../render/TextureManager.h"
#include "TEXT/TextManager.h"
#include "../ECS/Components.h"
#include "../PHYSICS/Collision.h"

Menu::Menu() {
    
}

Menu::~Menu() {
    this->e_entities.clear();
    this->_new_game.clear();
    this->_load_game.clear();
    this->_exit_game.clear();
    this->_enter_world_name.clear();
}

void Menu::init() {
    this->_generateE();
    this->_generateOptions();
}

void Menu::update() {

    static bool _e = false;

    /* automatically triggers next typing event upon [ENTER] */
    if (this->_entering_world_name && TextManager::freeKeyboardEnter(360, 330, "FG_WHITE_BG_BLACK")) {
        this->_entering_world_name = false;
        this->_generateOptions();
        for (auto & e : this->_enter_world_name) e->destroy();
        this->_enter_world_name.clear();
        _e = true;
    } else {
        _e = false;
    }

    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat && !_e) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            if (this->_selected_option == 0) {
                this->_selected_option = 1;
                TextManager::recolorText(this->_new_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_load_game, "FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 1) {
                this->_selected_option = 2;
                TextManager::recolorText(this->_load_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_exit_game, "FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 2) {
                this->_selected_option = 0;
                TextManager::recolorText(this->_exit_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_new_game, "FG_BRIGHTGREEN_BG_BLACK");
            }
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            if (this->_selected_option == 0) {
                this->_selected_option = 2;
                TextManager::recolorText(this->_new_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_exit_game, "FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 1) {
                this->_selected_option = 0;
                TextManager::recolorText(this->_load_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_new_game, "FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 2) {
                this->_selected_option = 1;
                TextManager::recolorText(this->_exit_game, "FG_WHITE_BG_BLACK");
                TextManager::recolorText(this->_load_game, "FG_BRIGHTGREEN_BG_BLACK");
            }
        } else if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            if (this->_selected_option == 0) {
                this->beginEnteringWorldName();
            } else if (this->_selected_option == 1) {
                std::cout << "lol\n";
            } else if (this->_selected_option == 2) {
                Game::_running = false;
            }
        }
    }
}

void Menu::beginEnteringWorldName() {

    std::cout << "begin entering world name\n";

    this->_clearOptions();

    this->_enter_world_name = TextManager::addText(320, 320, "FG_WHITE_BG_BLACK", "Name your world:");

    this->_entering_world_name = true;
}

void Menu::draw() {

}

void Menu::close() {
    for (auto & e : e_entities) e->destroy();
    e_entities.clear();

    for (auto & e : this->_new_game) e->destroy();
    this->_new_game.clear();

    for (auto & e : this->_load_game) e->destroy();
    this->_load_game.clear();

    for (auto & e : this->_exit_game) e->destroy();
    this->_exit_game.clear();
}

void Menu::_generateE() noexcept {

    /* The initial coords for letters */
    int x = 360;
    int y = 100;

    for (unsigned int i = 0u; i < 2u; ++i) {
        for (unsigned int j = 0u; j < 8u; ++j) {
            e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor("BLACK"), "e").front());
            x += 10;
        }

        y += 10;
        x = 360;
    }

    x = 340;
    y += 10;

    for (unsigned int i = 0u; i < 2u; ++i) {

        for (unsigned int j = 0u; j < 2u; ++j) {
            for (unsigned int k = 0u; k < 4u; ++k) {
                e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor("BLACK"), "e").front());
                x += 10;
            }
            x += 40;
        }

        y += 10;
        x = 340;
    }

    y += 10;
    x = 340;

    for (unsigned int i = 0u; i < 2u; ++i) {
        for (unsigned int j = 0u; j < 12u; ++j) {
            e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor("BLACK"), "e").front());
            x += 10;
        }

        y += 10;
        x = 340;
    }

    y += 10;
    x = 340;

    for (unsigned int i = 0u; i < 2u; ++i) {
        for (unsigned int j = 0u; j < 4u; ++j) {
            e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor("BLACK"), "e").front());
            x += 10;
        }

        y += 10;
        x = 340;
    }

    y += 10;
    x = 360;

    for (unsigned int i = 0u; i < 2u; ++i) {
        for (unsigned int j = 0u; j < 8u; ++j) {
            e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor("BLACK"), "e").front());
            x += 10;
        }

        y += 10;
        x = 360;
    }


}

void Menu::_generateOptions() noexcept {

    int x = 360;
    int y = 320;

    this->_new_game = TextManager::addText(x, y, "FG_BRIGHTGREEN_BG_BLACK", "New game");
    
    x = 355;
    y += 10;

    this->_load_game = TextManager::addText(x, y, "FG_WHITE_BG_BLACK", "Load game");

    x = 380;
    y += 10;

    this->_exit_game = TextManager::addText(x, y, "FG_WHITE_BG_BLACK", "Exit");

    this->_selected_option = 0;

}

void Menu::_clearOptions() noexcept {

    for (auto & e : _new_game) e->destroy();
    for (auto & e : _load_game) e->destroy();
    for (auto & e : _exit_game) e->destroy();
    this->_new_game.clear();
    this->_load_game.clear();
    this->_exit_game.clear();
}