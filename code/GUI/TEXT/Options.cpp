#include "Options.h"
#include "EntityString.h"

#include "../../game.h"
#include "../../ANSI/ANSI.h"
#include "TextManager.h"
#include "../../ECS/ECS.h"

Options::Options(const int &x, const int &y, const std::initializer_list<std::string> &option_strings) {
    /*
    for (unsigned int i = 0u; i < option_strings.size(); ++i) {
        if (i == this->_selected_option) 
            this->_options_entities.emplace_back(new EntityString(x, y + (i * 10), 0x0a, *(option_strings.begin() + i)));
        else 
            this->_options_entities.emplace_back(new EntityString(x, y + (i * 10), 0x0f, *(option_strings.begin() + i)));
    }
    */
    this->_x = x;
    this->_y = y;
    for (auto & s : option_strings)
        this->_options_strings.emplace_back(s);
    this->_num_options_to_display = this->_options_strings.size();
    this->_redrawOptionsEntities();
}

Options::Options(const int &x, const int &y, const unsigned int &num_options_to_display, const std::initializer_list<std::string> &option_strings) {
    this->_x = x;
    this->_y = y;
    this->_num_options_to_display = num_options_to_display;
    for (auto & s : option_strings)
        this->_options_strings.emplace_back(s);
    std::cout << "sizeof options_strings = " << this->_options_strings.size() << '\n';
    this->_redrawOptionsEntities();
}

Options::Options(const int &x, const int &y, const unsigned int &num_options_to_display, const std::initializer_list<std::string> &option_strings, const bool &x_centered) {
    this->_x = x;
    this->_y = y;
    this->_num_options_to_display = num_options_to_display;
    for (auto & s : option_strings)
        this->_options_strings.emplace_back(s);
    std::cout << "sizeof options_strings = " << this->_options_strings.size() << '\n';
    this->_x_centered = x_centered;
    this->_redrawOptionsEntities();
}

Options::~Options() {
    this->clear();
}

const int Options::update() {
    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            this->_options_entities[this->_selected_option]->recolor(0x0f);
            if (this->_selected_option == this->_options_entities.size() - 1) {
                this->_selected_option = 0;
            } else {
                ++this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor(0x0a);
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            this->_options_entities[this->_selected_option]->recolor(0x0f);
            if (this->_selected_option == 0) {
                this->_selected_option = this->_options_entities.size() - 1;
            } else {
                --this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor(0x0a);
        } else if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            return this->_selected_option;
        }
    }
    return -1;
}

/* a newer smarter breed of du */
const int Options::update2() noexcept {
    /*
    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            this->_options_entities[this->_selected_option]->recolor(0x0f);
            if (this->_selected_option == this->_options_entities.size() - 1) {
                this->_selected_option = 0;
            } else {
                ++this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor(0x0a);
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            this->_options_entities[this->_selected_option]->recolor(0x0f);
            if (this->_selected_option == 0) {
                this->_selected_option = this->_options_entities.size() - 1;
            } else {
                --this->_selected_option;
            }
            this->_options_entities[this->_selected_option]->recolor(0x0a);
        } else if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            return this->_selected_option;
        }
    }
    */

    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            std::cout << "Down event\n";
            /* The options revert to the top */
            if (this->_selected_option == this->_options_strings.size() - 1) {
                this->_top_option_displayed = this->_selected_option = 0u;
                this->_clearOptionsEntities();
                this->_redrawOptionsEntities();
            } else if (this->_selected_option == (this->_top_option_displayed + this->_num_options_to_display - 1)) {
                std::cout << "Shift options up event\n";
                this->_shiftOptionsUp();
                ++this->_top_option_displayed;
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_normal_option_color);
                ++this->_selected_option;
                //this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_selected_option_color);
                this->_adjustOptionsArrows();
            } else {
                std::cout << "Normal down event\n";
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_normal_option_color);
                ++this->_selected_option;
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_selected_option_color);
            }
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            std::cout << "Up event\n";
            /* The options loop around to the bottom */
            if (this->_selected_option == 0u) {
                this->_top_option_displayed = this->_options_strings.size() - this->_num_options_to_display;
                this->_selected_option = this->_options_strings.size() - 1;
                this->_clearOptionsEntities();
                this->_redrawOptionsEntities();
            } else if (this->_selected_option == this->_top_option_displayed) {
                std::cout << "Shift options down event\n";
                this->_shiftOptionsDown();
                --this->_top_option_displayed;
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_normal_option_color);
                --this->_selected_option;
                //this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_selected_option_color);
                this->_adjustOptionsArrows();
            } else {
                std::cout << "Normal up event\n";
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_normal_option_color);
                --this->_selected_option;
                this->_options_entities[this->_selected_option - this->_top_option_displayed]->recolor(this->_selected_option_color);
            }
        } else if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            return this->_selected_option;
        }
    }

    return -1;
}

void Options::draw() noexcept {
    for (auto & s : this->_options_entities) s->draw();
    this->_adjustOptionsArrows();
    this->_drawn = true;
}

void Options::hide() noexcept {
    for (auto & s : this->_options_entities) s->hide();
    if (this->_upArrow != nullptr) this->_hideArrow(this->_upArrow);
    if (this->_downArrow != nullptr) this->_hideArrow(this->_downArrow);
    this->_drawn = false;
}

void Options::clear() noexcept {
    this->_clearOptionsEntities();
    this->_deleteArrow(this->_upArrow);
    this->_deleteArrow(this->_downArrow);
}

void Options::_clearOptionsEntities() noexcept {
    for (auto & e : this->_options_entities) delete e;
    this->_options_entities.clear();
}

void Options::_redrawOptionsEntities() noexcept {
    for (unsigned int i = _top_option_displayed; i < std::min(this->_top_option_displayed + this->_num_options_to_display, (unsigned int)this->_options_strings.size()); ++i) {
        if (i == this->_selected_option) {
            this->_options_entities.emplace_back(new EntityString(
                this->_x_centered ? this->_x - ( TextManager::widthOfString(this->_options_strings[i], 10u) / 2u ) : this->_x, 
                this->_y + ((i - this->_top_option_displayed) * 10), this->_selected_option_color, this->_options_strings[i])
            );
        } else {
            this->_options_entities.emplace_back(new EntityString(
                this->_x_centered ? this->_x - ( TextManager::widthOfString(this->_options_strings[i], 10u) / 2u ) : this->_x, 
                this->_y + ((i - this->_top_option_displayed) * 10), this->_normal_option_color, this->_options_strings[i])
            );
        }
    }
    this->_adjustOptionsArrows();
}

void Options::_adjustOptionsArrows() noexcept {
    /* Redraw / create arrows as needed */
    if (this->_top_option_displayed != 0u) {
        if (this->_upArrow != nullptr) this->_drawArrow(this->_upArrow);
        else this->_createUpArrow();
    } else
        if (this->_upArrow != nullptr) this->_hideArrow(this->_upArrow);

    if (this->_top_option_displayed + this->_num_options_to_display < this->_options_strings.size()) {
        if (this->_downArrow != nullptr) this->_drawArrow(this->_downArrow);
        else this->_createDownArrow();
    } else 
        if (this->_downArrow != nullptr) this->_hideArrow(this->_downArrow);
}

void Options::_shiftOptionsUp() noexcept {
    /* Frontend move */
    for (unsigned int i = 1u; i < this->_num_options_to_display; ++i) this->_options_entities[i]->move(0, -10);
    /* Backend move */
    delete this->_options_entities.front();
    for (unsigned int i = 0u; i < this->_num_options_to_display - 1; ++i) this->_options_entities[i] = std::move(this->_options_entities[i+1]);
    //this->_options_entities.back() = new EntityString(this->_x, (this->_y + (this->_num_options_to_display - 1) * 10), 
    //    this->_selected_option_color, this->_options_strings[this->_selected_option + 1]);
    //this->_options_entities.emplace_back(new EntityString(
    //            this->_x_centered ? this->_x - ( TextManager::widthOfString(this->_options_strings[i], 10u) / 2u ) : this->_x, 
    //            this->_y + ((i - this->_top_option_displayed) * 10), this->_normal_option_color, this->_options_strings[i])
    //        );
    this->_options_entities.back() = new EntityString(
        this->_x_centered ? this->_x - ( TextManager::widthOfString(this->_options_strings[this->_selected_option + 1], 10u) / 2u ) : this->_x,
        this->_y + (this->_num_options_to_display - 1) * 10, this->_selected_option_color, this->_options_strings[this->_selected_option + 1]
    );
}

void Options::_shiftOptionsDown() noexcept {
    /* Frontend move */
    for (unsigned int i = 0u; i < this->_num_options_to_display - 1u; ++i) this->_options_entities[i]->move(0, 10);
    /* Backend move */
    delete this->_options_entities.back();
    for (unsigned int i = this->_options_entities.size() - 1; i > 0; --i) this->_options_entities[i] = std::move(this->_options_entities[i-1]);
    //this->_options_entities.front() = new EntityString(this->_x, this->_y, 
    //    this->_selected_option_color, this->_options_strings[this->_selected_option - 1]);
    this->_options_entities.front() = new EntityString(
        this->_x_centered ? this->_x - ( TextManager::widthOfString(this->_options_strings[this->_selected_option - 1], 10u) / 2u ) : this->_x,
        this->_y, this->_selected_option_color, this->_options_strings[this->_selected_option - 1]
    );
}

void Options::_createUpArrow() {
    this->_upArrow = TextManager::addText(this->_x - 5, this->_y - 10, this->_up_arrow_color, ANSI::CHARACTERS::ARROW_UP);
}

void Options::_createDownArrow() {
    this->_downArrow = TextManager::addText(this->_x - 5, this->_y + (this->_num_options_to_display * 10), this->_down_arrow_color, ANSI::CHARACTERS::ARROW_DOWN);
}

inline void Options::_drawArrow(Entity *arrow) noexcept { arrow->addGroup(groupLabels::groupPlayers); }

inline void Options::_hideArrow(Entity *arrow) noexcept { arrow->delGroup(groupLabels::groupPlayers); }

void Options::_deleteArrow(Entity *&arrow) noexcept {
    if (arrow != nullptr) {
        arrow->destroy();
        arrow = nullptr;
    }
}



