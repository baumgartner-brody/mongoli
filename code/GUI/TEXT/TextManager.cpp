#include "TextManager.h"

#include "../../ECS/Components.h"

int TextManager::TEXT_POSITION_X;
int TextManager::TEXT_POSITION_Y;
std::string *TextManager::freeKeyboardEntry = new std::string;
std::vector<Entity*> *TextManager::freeKeyboardDisplayText = new std::vector<Entity*>;
bool *TextManager::CAPS = new bool(false);

/* Initializes all text-assets */
void TextManager::init() {

    /* The c_in vector remains constant for every color combination */
    std::vector<SDL_Color> c_in;
    c_in.emplace_back(ANSI::SDLCOLOR::BLACK);
    c_in.emplace_back(ANSI::SDLCOLOR::WHITE);

    for (auto bg_itr = ANSI::SDLCOLOR::begin(); bg_itr != ANSI::SDLCOLOR::end(); ++bg_itr) {
        for (auto fg_itr = ANSI::SDLCOLOR::begin(); fg_itr != ANSI::SDLCOLOR::end(); ++fg_itr) {
            
            /* Do not create assets when bg_color == fg_color */
            if (*fg_itr == *bg_itr) continue;

            /* Build the name of this asset */
            std::string assetName = "FG_" + fg_itr.getColorName() + "_BG_" + bg_itr.getColorName();

            /* Build the c_out vector */
            std::vector<SDL_Color> c_out;
            c_out.emplace_back(*bg_itr);
            c_out.emplace_back(*fg_itr);

            /* Create the corresponding asset */
            Game::assetManager->addAsset(assetName, TextureManager::TextureTools::createRecoloredTexture(FONT_TEXTURE_FILE, c_in, c_out));

            /* Debug */
            std::cout << "Created asset \"" << assetName << "\"\n";

        }
    }
}

/* Frees all associated memory */
void TextManager::free() {
    TextManager::freeKeyboardEntry->clear();
    TextManager::freeKeyboardDisplayText->clear();
    delete TextManager::freeKeyboardDisplayText;
    delete TextManager::freeKeyboardEntry;
}

/* Creates the appropriate SDL_Rect for an 8-bit char int */
SDL_Rect TextManager::createSourceRect(const Uint8 &c) {

    /* Create the rect to adjust and return */
    SDL_Rect r = { 0, 0, 10, 10 };

    /* ☺ - ☼ */
    if (c <= Uint8(15)) {
        r.y = 0;
        r.x = c * 10;
    } else if (c <= Uint8(31)) {
        r.y = 10;
        r.x = (c - 15) * 10;
    } else if (c <= Uint8('/')) {
        r.y = 20;
        r.x = (c - ' ') * 10;
    } else if (c <= Uint8('?')) {
        r.y = 30;
        r.x = ( (c - '0') * 10 );
    } else if (c == Uint8('@')) {
        r.y = 70;
        r.x = 130;
    } else if (c <= Uint8('Z')) {
        r.y = 40 + ( ((c - 'A') / 13) * 10 );
        r.x = ( ((c - 'A') % 13) * 10 );
    } else if (c <= Uint8('`')) {
        r.y = 40 + ( ((c - 'Z') / 3) * 10 );
        r.x = 130 + ( ((c - 'Z') % 3) * 10 );
    } else if (c <= Uint8('z')) {
        r.y = 60 + ( ((c - 'a') / 13) * 10 );
        r.x = ( ((c - 'a') % 13) * 10 );
    } else if (c <= Uint8('}')) {
        r.y = 60 + ( ((c - 'z') / 3) * 10 );
        r.x = 130 + ( ((c - 'z') % 3) * 10 );
    } else if (c <= Uint8('~')) {
        r.y = 70;
        r.x = 140;
    /* 127 is omitted [DEL], which becomes Δ */
    } else if (c == Uint8(127)) {
        r.y = 70;
        r.x = 150;
    } else if (c >= Uint8(128) && c <= Uint8(143)) {
        r.y = 80;
        r.x = (c - 128) * 10;
    } else if (c <= Uint8(159)) {
        r.y = 90;
        r.x = (c - 144) * 10;
    } else if (c <= Uint8(175)) {
        r.y = 100;
        r.x = (c - 160) * 10;
    } else if (c <= Uint8(191)) {
        r.y = 110;
        r.x = (c - 176) * 10;
    } else if (c <= Uint8(207)) {
        r.y = 120;
        r.x = (c - 192) * 10;
    } else if (c <= Uint8(223)) {
        r.y = 130;
        r.x = (c - 208) * 10;
    } else if (c <= Uint8(239)) {
        r.y = 140;
        r.x = (c - 224) * 10;
    } else if (c <= Uint8(255)) {
        r.y = 150;
        r.x = (c - 240) * 10;
    }

    return r;
}

/* Adds text at the current TextManager position with the desired asset name */
/* The resultant letter entities are returned */
std::vector<Entity*> TextManager::addText(int x, int y, const std::string &assetName, const std::string &text) {

    /* The vector to return */
    std::vector<Entity*> v;

    int _TEXT_POSITION_X_TMP_ = x;
    int _TEXT_POSITION_Y_TMP_ = y;
    
    /* Add a letter entity for each desired letter */
    for (auto & l : text) {

        if (l == '\r') {
            x = _TEXT_POSITION_X_TMP_;
            continue;
        } else if (l == '\n') {
            x = _TEXT_POSITION_X_TMP_;
            y += 10;
            continue;
        } else if (l == ' ') {
            x += 10;
            continue;
        }

        Entity *e = &Game::manager->addEntity();
        e->addComponent<TransformComponent>(x, y, 10, 10);
        e->addComponent<SpriteComponent>(assetName, TextManager::createSourceRect(l));
        e->addGroup(0u);
        v.emplace_back(e);

        /* Move the next letter's position */
        x += 10;
    }

    return v;
}

/* Given a vector of letter entities, iterate thru and recolor them. */
void TextManager::recolorText(std::vector<Entity*> &text_to_recolor, const std::string &newColorAssetName) {
    for (auto & e : text_to_recolor)
        e->getComponent<SpriteComponent>().swapAsset(newColorAssetName);
}

/* Generates a random foreground color to go with the provided background color */
const std::string TextManager::generateRandomTextColor(const std::string &BG_COLOR) {

    int r;

    do {
        r = std::rand() % 16;
    } while (ANSI::SDLCOLOR_LOOKUP::NAME[r] == BG_COLOR);
    
    std::string s = "FG_" + ANSI::SDLCOLOR_LOOKUP::NAME[r] + "_BG_" + BG_COLOR;

    return s;
}

/* Allows the user to type until an [ENTER] key is detected */
const bool TextManager::freeKeyboardEnter(int x, int y, const std::string &assetName) {

    int t = SDL_GetModState();
    if ((t & KMOD_CAPS) == KMOD_CAPS) {
        *TextManager::CAPS = true;
    } else {
        *TextManager::CAPS = false;
    }

    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_RETURN || Game::event->key.keysym.sym == SDLK_RETURN2) {
            return true;
        } else if (Game::event->key.keysym.sym >= 'a' && Game::event->key.keysym.sym <= 'z') {
            if (*TextManager::CAPS) {
                *TextManager::freeKeyboardEntry += static_cast<char>(Game::event->key.keysym.sym - 32);
            } else {
                *TextManager::freeKeyboardEntry += static_cast<char>(Game::event->key.keysym.sym);
            }
            for (auto & e : *TextManager::freeKeyboardDisplayText) e->destroy();
            TextManager::freeKeyboardDisplayText->clear();
            *TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetName, *TextManager::freeKeyboardEntry);
        } else if (Game::event->key.keysym.sym == SDLK_BACKSPACE && !TextManager::freeKeyboardEntry->empty()) {
            TextManager::freeKeyboardEntry->pop_back();
            for (auto & e : *TextManager::freeKeyboardDisplayText) e->destroy();
            TextManager::freeKeyboardDisplayText->clear();
            *TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetName, *TextManager::freeKeyboardEntry);
        } else if (Game::event->key.keysym.sym == SDLK_LSHIFT || Game::event->key.keysym.sym == SDLK_RSHIFT || Game::event->key.keysym.sym == SDLK_CAPSLOCK) {
            std::cout << "Triggered shift event\n";
            *TextManager::CAPS = !(*TextManager::CAPS);
        } else if (Game::event->key.keysym.sym == SDLK_SPACE) {
            *TextManager::freeKeyboardEntry += ' ';
        }
    } else if (Game::event->type == SDL_KEYUP) {
        if (Game::event->key.keysym.sym == SDLK_LSHIFT || Game::event->key.keysym.sym == SDLK_RSHIFT) {
            std::cout << "SHIFT WAS RELEASED\n";
            *TextManager::CAPS = !(*TextManager::CAPS);
        }
    }

    return false;

}