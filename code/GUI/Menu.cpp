#include "Menu.h"

#include "../render/TextureManager.h"
#include "TEXT/TextManager.h"
#include "TEXT/Options.h"
#include "../ECS/Components.h"
#include "../GEN/Worldgen.h"

Menu::Menu() {
    
}

Menu::~Menu() {
    this->e_entities.clear();
    this->_clearOptions();
    this->_enter_world_name.clear();
}

void Menu::init() {
    this->_generateE();
    this->_generateOptions();

    /* _TESTING_ */
    //this->_TESTFUNCTION_DRAW_INN_ROOM1();
}

void Menu::update() {

    if (this->_entering_world_name) {
        if (TextManager::freeKeyboardEnter2(0, 0, 0x0e)) {
            std::cout << "No longer entering text\n";
            Game::_running = false;
            this->_entering_world_name = false;
        }
        return;
    }
    
    // TODO 
    /* automatically triggers next typing event upon [ENTER] */
    /*if (this->_entering_world_name && TextManager::freeKeyboardEnter2(360, 330, 0x0f)) {
        this->_entering_world_name = false;
        for (auto & e : this->_enter_world_name) e->destroy();
        this->_enter_world_name.clear();
        WorldGen::writeToFile("src/saves/test.txt", *TextManager::freeKeyboardEntry);
        TextManager::destroyFreeKeyboardEntry();
        std::cout << "List of directories in \"src\\saves\":\n";
        //for (auto & s : WorldGen::getDirectories("src\\saves")) std::cout << "\"" << s << "\"\n";
        _e = true;
        // return; (instead of static bool _e)
    } else {
        _e = false;
    }
    */

    int r = -1;
    if (this->_options->isDrawn()) {
        r = this->_options->update2();
    }

    // TESTING OPTIONS HIDE-AND-REDRAWABILITY
    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat && Game::event->key.keysym.sym == SDLK_ESCAPE) {
        if (this->_options->isDrawn()) this->_options->hide();
        else this->_options->draw();
    }

    if (r == 0) {
        std::cout << "New world?\n";
        this->beginEnteringWorldName();
    } else if (r == 1)
        std::cout << "Load?\n";
    else if (r == 2) 
        Game::_running = false;
    else if (r == 3)
        Game::_running = false;

    /*
    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat && !_e) {
        if (Game::event->key.keysym.sym == SDLK_DOWN) {
            if (this->_selected_option == 0) {
                this->_selected_option = 1;
                //TextManager::recolorText(this->_new_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_load_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_new_game->recolor("FG_WHITE_BG_BLACK");
                this->_load_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 1) {
                this->_selected_option = 2;
                //TextManager::recolorText(this->_load_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_exit_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_load_game->recolor("FG_WHITE_BG_BLACK");
                this->_exit_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 2) {
                this->_selected_option = 0;
                //TextManager::recolorText(this->_exit_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_new_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_exit_game->recolor("FG_WHITE_BG_BLACK");
                this->_new_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
            }
        } else if (Game::event->key.keysym.sym == SDLK_UP) {
            if (this->_selected_option == 0) {
                this->_selected_option = 2;
                //TextManager::recolorText(this->_new_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_exit_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_new_game->recolor("FG_WHITE_BG_BLACK");
                this->_exit_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 1) {
                this->_selected_option = 0;
                //TextManager::recolorText(this->_load_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_new_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_load_game->recolor("FG_WHITE_BG_BLACK");
                this->_new_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
            } else if (this->_selected_option == 2) {
                this->_selected_option = 1;
                //TextManager::recolorText(this->_exit_game, "FG_WHITE_BG_BLACK");
                //TextManager::recolorText(this->_load_game, "FG_BRIGHTGREEN_BG_BLACK");
                this->_exit_game->recolor("FG_WHITE_BG_BLACK");
                this->_load_game->recolor("FG_BRIGHTGREEN_BG_BLACK");
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
    */
}

void Menu::beginEnteringWorldName() {

    std::cout << "begin entering world name\n";

    // TODO - Redraw options if user ESC's out of world name creation
    this->_options->hide();

    this->_enter_world_name = TextManager::addText(320, 320, 0x0f, "Name your world:");

    TextManager::getCAPSState();

    SDL_StartTextInput();

    this->_entering_world_name = true;
}

void Menu::draw() {

}

void Menu::close() {
    for (auto & e : e_entities) e->destroy();
    e_entities.clear();

    this->_clearOptions();
}

void Menu::_generateE() noexcept {

    /* The initial coords for letters */
    int x = 360;
    int y = 100;

    const int vertical_spacer = 5;

    this->_generateESection(x, y, 8, 2, 360);

    x = 340;
    y += vertical_spacer;

    for (unsigned int i = 0u; i < 2u; ++i) {

        for (unsigned int j = 0u; j < 2u; ++j) {
            for (unsigned int k = 0u; k < 4u; ++k) {
                e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor(0x00), "e").front());
                x += 10;
            }
            x += 40;
        }

        y += 10;
        x = 340;
    }

    y += vertical_spacer;
    x = 340;

    this->_generateESection(x, y, 12, 2, 340);

    y += vertical_spacer;
    x = 340;

    this->_generateESection(x, y, 4, 2, 340);

    y += vertical_spacer;
    x = 360;
    
    this->_generateESection(x, y, 8, 2, 360);
}

void Menu::_generateESection(int &x, int &y, const unsigned int &w, const unsigned int &h, const int &initial_x) noexcept {
    for (unsigned int i = 0u; i < h; ++i) {
        for (unsigned int j = 0u; j < w; ++j) {
            e_entities.emplace_back(TextManager::addText(x, y, TextManager::generateRandomTextColor(0x00), "e").front());
            x += 10;
        }

        y += 10;
        x = initial_x;
    }
}

void Menu::_generateOptions() noexcept {

    /*
    int x = 360;
    int y = 320;

    this->_new_game = new EntityString(x, y, "FG_BRIGHTGREEN_BG_BLACK", "New game");
    
    x = 355;
    y += 10;

    this->_load_game = new EntityString(x, y, "FG_WHITE_BG_BLACK", "Load game");

    x = 380;
    y += 10;

    this->_exit_game = new EntityString(x, y, "FG_WHITE_BG_BLACK", "Exit");

    this->_selected_option = 0;
    */
    this->_options = new Options(400, 320, 3, {"New game☻", "♦Load game♦", "Settings♥", "Options☻", "Exit☺"}, true);

}

void Menu::_clearOptions() noexcept {

    delete this->_options;
    this->_options = nullptr;
}

void Menu::_TESTFUNCTION_DRAW_INN_ROOM1() noexcept {

    std::cout << "Called _TEST\n";

    int x = 500;
    int y = 500;

    TextManager::addText(x, y, 0x07, Uint8(ANSI::CHARACTERS::BOX_DRAWING_CORNER_TOP_LEFT_DOUBLE));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(ANSI::CHARACTERS::BOX_DRAWING_HORIZONTAL_DOUBLE));
    x += 10;
    TextManager::addText(x, y, 0x06, Uint8(186));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(205));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(ANSI::CHARACTERS::BOX_DRAWING_CORNER_TOP_RIGHT_DOUBLE));

    y += 10;
    x = 500;

    TextManager::addText(x, y, 0x07, Uint8(ANSI::CHARACTERS::TILDE));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(ANSI::CHARACTERS::SOLID));
    
    y += 10;
    x = 500;

    TextManager::addText(x, y, 0x07, Uint8(186));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(186));

    y += 10;
    x = 500;

    TextManager::addText(x, y, 0x07, Uint8(186));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x08, Uint8('+'));
    x += 10;
    TextManager::addText(x, y, 0x06, Uint8(233));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(186));

    y += 10;
    x = 500;

    TextManager::addText(x, y, 0x07, Uint8(200));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(205));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(205));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(205));
    x += 10;
    TextManager::addText(x, y, 0x07, Uint8(188));

}