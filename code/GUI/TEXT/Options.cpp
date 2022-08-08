#include "Options.h"
#include "EntityString.h"

#include "../../game.h"
#include "../../ANSI/ANSI.h"

Options::Options() {

}

Options::Options(const int &x, const int &y, const std::initializer_list<std::string> &option_strings) {
    for (unsigned int i = 0u; i < option_strings.size(); ++i) {
        if (i == this->_selected_option) 
            this->_options_entities.emplace_back(new EntityString(x, y + (i * 10), "FG_BRIGHTGREEN_BG_BLACK", *(option_strings.begin() + i)));
        else 
            this->_options_entities.emplace_back(new EntityString(x, y + (i * 10), "FG_WHITE_BG_BLACK", *(option_strings.begin() + i)));
    }
}

Options::~Options() {
    for (auto & s : this->_options_entities) delete s;
    this->_options_entities.clear();
}

const int Options::update() {
    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            this->_options_entities[this->_selected_option]->recolor("FG_WHITE_BG_BLACK");
            if (this->_selected_option == this->_options_entities.size() - 1) {
                this->_selected_option = 0;
            } else {
                ++this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor("FG_BRIGHTGREEN_BG_BLACK");
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            this->_options_entities[this->_selected_option]->recolor("FG_WHITE_BG_BLACK");
            if (this->_selected_option == 0) {
                this->_selected_option = this->_options_entities.size() - 1;
            } else {
                --this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor("FG_BRIGHTGREEN_BG_BLACK");
        } else if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            return this->_selected_option;
        }
    }
    return -1;
}



