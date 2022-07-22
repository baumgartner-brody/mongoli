#include "TextManager.h"

#include "../ECS/Components.h"

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