#include "TextManager.h"

#include "../../ECS/Components.h"
#include "TypedText.h"

/* TODO */
/* DEBUG - REMEMBER TO REMOVE */
#include <fstream>
/* */

/* TODO - TypedText object should not be part of TextManager */
/*  It should be passed into freeKeyboardEnter2 via pointer / reference */

/* TODO - Add a class for drawing an announcement / text box */
/* TODO - Options should be a base class that allows for both horizontal and vertical displays and interfaces */
/* TODO - Methods for queueing up text / display text with multiple colors */
// additional ctors for options and typedtext

std::string freeKeyboardText;
std::string composition;
Sint32 cursor;
Sint32 selection_len;

/* Declarations for all static and extern members of TextManager */
// TODO - Most of these variables are unused
int TextManager::TEXT_POSITION_X;
int TextManager::TEXT_POSITION_Y;
std::string *TextManager::freeKeyboardEntry = new std::string;
std::vector<Entity*> *TextManager::freeKeyboardDisplayText = new std::vector<Entity*>;
bool *TextManager::CAPS = new bool(false);
std::vector<Uint8> TextManager::freeKeyboardSymbols;

// TODO - See above note
TypedText *TextManager::freeKeyboardTypedText = new TypedText(0, 0);

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

            /* Build the hex color code of this asset */
            //std::string assetName = "FG_" + fg_itr.getColorName() + "_BG_" + bg_itr.getColorName();   
            Uint8 assetNumber = ANSI::NUMERIC::createColorCode(fg_itr.numeric(), bg_itr.numeric());      

            /* Build the c_out vector */
            std::vector<SDL_Color> c_out;
            c_out.emplace_back(*bg_itr);
            c_out.emplace_back(*fg_itr);

            /* Create the corresponding asset */
            Game::assetManager->addAsset(assetNumber, TextureManager::TextureTools::createRecoloredTexture(FONT_TEXTURE_FILE, c_in, c_out));

            /* Debug */
            std::cout << "Created asset \"" << std::hex << (int)assetNumber << "\"\n";

        }
    }
}

/* Frees all associated memory */
void TextManager::free() {
    std::cout << "TextManager::free()\n";
    TextManager::freeKeyboardEntry->clear();
    TextManager::freeKeyboardDisplayText->clear();
    delete TextManager::freeKeyboardDisplayText;
    delete TextManager::freeKeyboardEntry;
    delete TextManager::freeKeyboardTypedText;
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
        r.x = (c - 16) * 10;
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
std::vector<Entity*> TextManager::addText(int x, int y, const Uint8 &assetNumber, const std::string &text) {

    /* The vector to return */
    std::vector<Entity*> v;

    int _TEXT_POSITION_X_TMP_ = x;
    int _TEXT_POSITION_Y_TMP_ = y;
    
    /* Add a letter entity for each desired letter */
    for (std::string::const_iterator l = std::begin(text); l != std::end(text); ++l) {

        if (*l == '\r') {
            x = _TEXT_POSITION_X_TMP_;
            continue;
        } else if (*l == '\n') {
            x = _TEXT_POSITION_X_TMP_;
            y += 10;
            continue;
        } else if (*l == ' ') {
            x += 10;
            continue;
        }

        Uint8 c = static_cast<Uint8>(*l);
        if (c == Uint8(0xe2) || c == Uint8(0xc2) || c == Uint8(0xc3) || c == Uint8(0xce) || c == Uint8(0xcf)) 
            v.emplace_back(TextManager::addText(x, y, assetNumber, TextManager::digestComplexCharacter(text, l, c)));
        else
            v.emplace_back(TextManager::addText(x, y, assetNumber, static_cast<Uint8>(*l)));

        /* Move the next letter's position */
        x += 10;
    }

    return v;
}

/* Overload of addText with std::vector<Uint8> to support extended ascii characters */
std::vector<Entity*> TextManager::addText(int x, int y, const Uint8 &assetNumber, const std::vector<Uint8> &symbols) {
    /* The vector to return */
    std::vector<Entity*> v;

    int _TEXT_POSITION_X_TMP_ = x;
    int _TEXT_POSITION_Y_TMP_ = y;
    
    /* Add a letter entity for each desired letter */
    for (auto & l : symbols) {

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

        v.emplace_back(TextManager::addText(x, y, assetNumber, l));

        /* Move the next letter's position */
        x += 10;
    }

    return v;
}

/* Adds the specified group of extended ascii characters at the specified position */
/* This method is specifically fit to TypedText's needs */
// TODO - \r and \n
std::vector<Entity*> TextManager::addTextWithHitboxes(int x, int y, const Uint8 &assetNumber, const std::vector<Uint8> &symbols) {
    /* The vector to return */
    std::vector<Entity*> v;

    int _TEXT_POSITION_X_TMP_ = x;
    int _TEXT_POSITION_Y_TMP_ = y;
    
    /* Add a letter entity for each desired letter */
    for (auto & l : symbols) {

        if (l == '\r') {
            x = _TEXT_POSITION_X_TMP_;
            continue;
        } else if (l == '\n') {
            x = _TEXT_POSITION_X_TMP_;
            y += 10;
            continue;
        } 
        /* else if (l == ' ') {
            x += 10;
            continue;
        } */

        v.emplace_back(TextManager::addText(x, y, assetNumber, l));
        v.back()->addComponent<HitboxComponent>(x, y, 10, 10);

        /* Move the next letter's position */
        x += 10;
    }

    return v;
}

/* Adds a singular character entity at the desired location */
Entity* TextManager::addText(int x, int y, const Uint8 &assetNumber, const Uint8 &c) {
    Entity *e = &Game::manager->addEntity();
    e->addComponent<TransformComponent>(x, y, 10, 10);
    e->addComponent<SpriteComponent>(assetNumber, TextManager::createSourceRect(c));
    e->addGroup(0u);
    return e;
}

// The itr may be moved 
const Uint8 TextManager::digestComplexCharacter(const std::string &text, std::string::const_iterator &itr, const Uint8 &first) {
    std::cout << "Called Textmanager::digest()\n";
    /* Grab up to two subsequent chars after the delimeter */
    ++itr;
    /* Malformed complex character */
    if (itr == std::end(text)) {
        std::cerr << "MALFORMED 1\n";
        exit(-1);
    } 
    Uint8 c1 = *itr;

    Uint8 r = _d1(first, c1);
    if (r != Uint8(0u))
        return r;

    ++itr;
    /* Malformed complex character */
    if (itr == std::end(text)) {
        std::cerr << "MALFORMED 2\n";
        exit(-1);
    } 
    Uint8 c2 = *itr;

    std::cout << "c1: " << std::hex << (int)c1 << '\n';
    std::cout << "c2: " << std::hex << (int)c2 << '\n';    

    return _d2(c1, c2);
}

/* Wrapper of digestComplexCharacter with index instead of itr */
const Uint8 TextManager::digestComplexCharacter(const char *text, unsigned int &index, const Uint8 &first) {
    std::cout << "Called Textmanager::digest() (char *)\n";
    /* Grab up to two subsequent chars after the delimeter */
    ++index;
    /* Malformed complex character */
    if (text[index] == '\0') {
        std::cerr << "MALFORMED 1\n";
        exit(-1);
    } 
    Uint8 c1 = text[index];

    Uint8 r = _d1(first, c1);
    if (r != Uint8(0u))
        return r;

    ++index;
    /* Malformed complex character */
    if (text[index] == '\0') {
        std::cerr << "MALFORMED 2\n";
        exit(-1);
    } 
    Uint8 c2 = text[index];

    std::cout << "c1: " << std::hex << (int)c1 << '\n';
    std::cout << "c2: " << std::hex << (int)c2 << '\n';    

    return _d2(c1, c2);
}

const Uint8 TextManager::_d1(const Uint8 &first, const Uint8 &c1) {
    /* Delims with 1 trailing char */
    if (first == Uint8(0xc2)) {
        if (c1 == Uint8(0xb6)) return Uint8(20u);
        else if (c1 == Uint8(0xa7)) return Uint8(21u);
        else if (c1 == Uint8(0xa2)) return Uint8(155u);
        else if (c1 == Uint8(0xa3)) return Uint8(156u);
        else if (c1 == Uint8(0xa5)) return Uint8(157u);
        else if (c1 == Uint8(0xaa)) return Uint8(166u);
        else if (c1 == Uint8(0xba)) return Uint8(167u);
        else if (c1 == Uint8(0xbf)) return Uint8(168u);
        else if (c1 == Uint8(0xac)) return Uint8(170u);
        else if (c1 == Uint8(0xbd)) return Uint8(171u);
        else if (c1 == Uint8(0xbc)) return Uint8(172u);
        else if (c1 == Uint8(0xa1)) return Uint8(173u);
        else if (c1 == Uint8(0xab)) return Uint8(174u);
        else if (c1 == Uint8(0xbb)) return Uint8(175u);
        else if (c1 == Uint8(0xb5)) return Uint8(230u);
        else if (c1 == Uint8(0xb1)) return Uint8(241u);
        else if (c1 == Uint8(0xb0)) return Uint8(248u);
        else if (c1 == Uint8(0xb7)) return Uint8(250u);
        else if (c1 == Uint8(0xb2)) return Uint8(253u);
        else if (c1 == Uint8(0xa0)) return Uint8(255u);
    } else if (first == Uint8(0xc3)) {
        if (c1 == Uint8(0x87)) return Uint8(128u);
        else if (c1 == Uint8(0xbc)) return Uint8(129u);
        else if (c1 == Uint8(0xa9)) return Uint8(130u);
        else if (c1 == Uint8(0xa2)) return Uint8(131u);
        else if (c1 == Uint8(0xa4)) return Uint8(132u);
        else if (c1 == Uint8(0xa0)) return Uint8(133u);
        else if (c1 == Uint8(0xa5)) return Uint8(134u);
        else if (c1 == Uint8(0xa7)) return Uint8(135u);
        else if (c1 == Uint8(0xaa)) return Uint8(136u); 
        else if (c1 == Uint8(0xab)) return Uint8(137u);
        else if (c1 == Uint8(0xa8)) return Uint8(138u);
        else if (c1 == Uint8(0xaf)) return Uint8(139u);
        else if (c1 == Uint8(0xae)) return Uint8(140u);
        else if (c1 == Uint8(0xac)) return Uint8(141u);
        else if (c1 == Uint8(0x84)) return Uint8(142u);
        else if (c1 == Uint8(0x85)) return Uint8(143u);
        else if (c1 == Uint8(0x89)) return Uint8(144u);
        else if (c1 == Uint8(0xa6)) return Uint8(145u);
        else if (c1 == Uint8(0x86)) return Uint8(146u);
        else if (c1 == Uint8(0xb4)) return Uint8(147u);
        else if (c1 == Uint8(0xb6)) return Uint8(148u);
        else if (c1 == Uint8(0xb2)) return Uint8(149u);
        else if (c1 == Uint8(0xbb)) return Uint8(150u);
        else if (c1 == Uint8(0xb9)) return Uint8(151u);
        else if (c1 == Uint8(0xbf)) return Uint8(152u);
        else if (c1 == Uint8(0x96)) return Uint8(153u);
        else if (c1 == Uint8(0x9c)) return Uint8(154u);
        else if (c1 == Uint8(0xa1)) return Uint8(160u);
        else if (c1 == Uint8(0xad)) return Uint8(161u);
        else if (c1 == Uint8(0xb3)) return Uint8(162u);
        else if (c1 == Uint8(0xba)) return Uint8(163u);
        else if (c1 == Uint8(0xb1)) return Uint8(164u);
        else if (c1 == Uint8(0x91)) return Uint8(165u);
        else if (c1 == Uint8(0x9f)) return Uint8(225u);
        else if (c1 == Uint8(0xb7)) return Uint8(246u);
    } else if (first == Uint8(0xc6)) {
        if (c1 == Uint8(0x92)) return Uint8(159u);
    } else if (first == Uint8(0xce)) {
        if (c1 == Uint8(0xb1)) return Uint8(224u);
        else if (c1 == Uint8(0x93)) return Uint8(226u);
        else if (c1 == Uint8(0xa3)) return Uint8(228u);
        else if (c1 == Uint8(0xa6)) return Uint8(232u);
        else if (c1 == Uint8(0x98)) return Uint8(233u);
        else if (c1 == Uint8(0xa9)) return Uint8(234u);
        else if (c1 == Uint8(0xb4)) return Uint8(235u);
        else if (c1 == Uint8(0xb5)) return Uint8(238u);
    } else if (first == Uint8(0xcf)) {
        if (c1 == Uint8(0x80)) return Uint8(227u);
        else if (c1 == Uint8(0x83)) return Uint8(229u);
        else if (c1 == Uint8(0x84)) return Uint8(231u);
        else if (c1 == Uint8(0x86)) return Uint8(237u);
    } 

    return Uint8(0u);
}

const Uint8 TextManager::_d2(const Uint8 &c1, const Uint8 &c2) {
    /* Delims with 2 trailing chars */
    if (c1 == Uint8(0x98)) {
        if (c2 == Uint8(0xba)) return Uint8(1u);
        else if (c2 == Uint8(0xbb)) return Uint8(2u);
        else if (c2 == Uint8(0xbc)) return Uint8(15u);
    } else if (c1 == Uint8(0x99)) {
        if (c2 == Uint8(0xa5)) return Uint8(3u);
        else if (c2 == Uint8(0xa6)) return Uint8(4u);
        else if (c2 == Uint8(0xa3)) return Uint8(5u);
        else if (c2 == Uint8(0xa0)) return Uint8(6u);
        else if (c2 == Uint8(0x82)) return Uint8(11u);
        else if (c2 == Uint8(0x80)) return Uint8(12u);
        else if (c2 == Uint8(0xaa)) return Uint8(13u);
        else if (c2 == Uint8(0xab)) return Uint8(14u);
    } else if (c1 == Uint8(0x80)) {
        if (c2 == Uint8(0xa2)) return Uint8(7u);
        else if (c2 == Uint8(0xbc)) return Uint8(19u);
    } else if (c1 == Uint8(0x97)) {
        if (c2 == Uint8(0x98)) return Uint8(8u);
        else if (c2 == Uint8(0x8b)) return Uint8(9u);
        else if (c2 == Uint8(0x99)) return Uint8(10u);
        else if (c2 == Uint8(0x84)) return Uint8(17u);
    } else if (c1 == Uint8(0x96)) {
        if (c2 == Uint8(0xba)) return Uint8(16u);
        else if (c2 == Uint8(0xac)) return Uint8(22u);
        else if (c2 == Uint8(0xb2)) return Uint8(30u);
        else if (c2 == Uint8(0xbc)) return Uint8(31u);
        else if (c2 == Uint8(0x91)) return Uint8(176u);
        else if (c2 == Uint8(0x92)) return Uint8(177u);
        else if (c2 == Uint8(0x93)) return Uint8(178u);
        else if (c2 == Uint8(0x88)) return Uint8(219u);
        else if (c2 == Uint8(0x84)) return Uint8(220u);
        else if (c2 == Uint8(0x8c)) return Uint8(221u);
        else if (c2 == Uint8(0x90)) return Uint8(222u);
        else if (c2 == Uint8(0x80)) return Uint8(223u);
        else if (c2 == Uint8(0xa0)) return Uint8(254u);
    } else if (c1 == Uint8(0x86)) {
        if (c2 == Uint8(0x95)) return Uint8(18u);
        else if (c2 == Uint8(0xa8)) return Uint8(23u);
        else if (c2 == Uint8(0x91)) return Uint8(24u);
        else if (c2 == Uint8(0x93)) return Uint8(25u);
        else if (c2 == Uint8(0x92)) return Uint8(26u);
        else if (c2 == Uint8(0x90)) return Uint8(27u);
        else if (c2 == Uint8(0x94)) return Uint8(29u);
    } else if (c1 == Uint8(0x88)) {
        if (c2 == Uint8(0x9f)) return Uint8(28u);
        else if (c2 == Uint8(0x9e)) return Uint8(236u);
        else if (c2 == Uint8(0xa9)) return Uint8(239u);
        else if (c2 == Uint8(0x99)) return Uint8(249u);
        else if (c2 == Uint8(0x9a)) return Uint8(251u);
    } else if (c1 == Uint8(0x8c)) {
        if (c2 == Uint8(0x82)) return Uint8(127u);
        else if (c2 == Uint8(0x90)) return Uint8(169u); 
        else if (c2 == Uint8(0xa0)) return Uint8(244u); 
        else if (c2 == Uint8(0xa1)) return Uint8(245u); 
    } else if (c1 == Uint8(0x82)) {
        if (c2 == Uint8(0xa7)) return Uint8(158u);
    } else if (c1 == Uint8(0x94)) {
        if (c2 == Uint8(0x82)) return Uint8(179u);
        else if (c2 == Uint8(0xa4)) return Uint8(180u); 
        else if (c2 == Uint8(0x90)) return Uint8(191u);
        else if (c2 == Uint8(0x94)) return Uint8(192u);
        else if (c2 == Uint8(0xb4)) return Uint8(193u);
        else if (c2 == Uint8(0xac)) return Uint8(194u);
        else if (c2 == Uint8(0x9c)) return Uint8(195u);
        else if (c2 == Uint8(0x80)) return Uint8(196u);
        else if (c2 == Uint8(0xbc)) return Uint8(197u);
        else if (c2 == Uint8(0x98)) return Uint8(217u);
        else if (c2 == Uint8(0x8c)) return Uint8(218u);
    } else if (c1 == Uint8(0x95)) {
        if (c2 == Uint8(0xa1)) return Uint8(181u);
        else if (c2 == Uint8(0xa2)) return Uint8(182u);
        else if (c2 == Uint8(0x96)) return Uint8(183u);
        else if (c2 == Uint8(0x95)) return Uint8(184u);
        else if (c2 == Uint8(0xa3)) return Uint8(185u);
        else if (c2 == Uint8(0x91)) return Uint8(186u);
        else if (c2 == Uint8(0x97)) return Uint8(187u);
        else if (c2 == Uint8(0x9d)) return Uint8(188u);
        else if (c2 == Uint8(0x9c)) return Uint8(189u);
        else if (c2 == Uint8(0x9b)) return Uint8(190u);
        else if (c2 == Uint8(0x9e)) return Uint8(198u);
        else if (c2 == Uint8(0x9f)) return Uint8(199u);
        else if (c2 == Uint8(0x9a)) return Uint8(200u);
        else if (c2 == Uint8(0x94)) return Uint8(201u);
        else if (c2 == Uint8(0xa9)) return Uint8(202u);
        else if (c2 == Uint8(0xa6)) return Uint8(203u);
        else if (c2 == Uint8(0xa0)) return Uint8(204u);
        else if (c2 == Uint8(0x90)) return Uint8(205u);
        else if (c2 == Uint8(0xac)) return Uint8(206u);
        else if (c2 == Uint8(0xa7)) return Uint8(207u);
        else if (c2 == Uint8(0xa8)) return Uint8(208u);
        else if (c2 == Uint8(0xa4)) return Uint8(209u);
        else if (c2 == Uint8(0xa5)) return Uint8(210u);
        else if (c2 == Uint8(0x99)) return Uint8(211u);
        else if (c2 == Uint8(0x98)) return Uint8(212u);
        else if (c2 == Uint8(0x92)) return Uint8(213u);
        else if (c2 == Uint8(0x93)) return Uint8(214u);
        else if (c2 == Uint8(0xab)) return Uint8(215u);
        else if (c2 == Uint8(0xaa)) return Uint8(216u);
    } else if (c1 == Uint8(0x89)) {
        if (c2 == Uint8(0xa1)) return Uint8(240u);
        else if (c2 == Uint8(0xa5)) return Uint8(242u);
        else if (c2 == Uint8(0xa4)) return Uint8(243u);
        else if (c2 == Uint8(0x88)) return Uint8(247u);
    } else if (c1 == Uint8(0x81)) {
        if (c2 == Uint8(0xbf)) return Uint8(252u);
    }

    // TODO - This should be 0 to signal an invalid character 
    return Uint8(1u);
}

/* Given a vector of letter entities, iterate thru and recolor them. */
void TextManager::recolorText(std::vector<Entity*> &text_to_recolor, const Uint8 &newAssetColorNumber) {
    for (auto & e : text_to_recolor)
        e->getComponent<SpriteComponent>().swapAsset(newAssetColorNumber);
}

/* Generates a random foreground color to go with the provided background color */
const Uint8 TextManager::generateRandomTextColor(const Uint8 &BG_COLOR) {

    int r;

    do {
        r = std::rand() % 16;
    } while (r == BG_COLOR);
    
    //std::string s = "FG_" + ANSI::SDLCOLOR_LOOKUP::NAME[r] + "_BG_" + BG_COLOR;
    return ANSI::NUMERIC::createColorCode(r, BG_COLOR);
}

void TextManager::getCAPSState() {
    int t = SDL_GetModState();
    if ((t & KMOD_CAPS) == KMOD_CAPS) {
        *TextManager::CAPS = true;
    } else {
        *TextManager::CAPS = false;
    }
}

/* Allows the user to type until an [ENTER] key is detected */
// TODO
// UNUSED - [SLATED FOR REMOVAL]
const bool TextManager::freeKeyboardEnter(int x, int y, const Uint8 &assetNumber) {    

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
            *TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetNumber, *TextManager::freeKeyboardEntry);
        } else if (Game::event->key.keysym.sym == SDLK_BACKSPACE && !TextManager::freeKeyboardEntry->empty()) {
            TextManager::freeKeyboardEntry->pop_back();
            for (auto & e : *TextManager::freeKeyboardDisplayText) e->destroy();
            TextManager::freeKeyboardDisplayText->clear();
            *TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetNumber, *TextManager::freeKeyboardEntry);
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

const bool TextManager::freeKeyboardEnter2(int x, int y, const Uint8 &assetNumber) {

    const Uint8* keyboard = SDL_GetKeyboardState(NULL);
    static unsigned int _LAST_BACKSPACE_TIME = 0u;

    if (keyboard[SDL_SCANCODE_BACKSPACE]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            _LAST_BACKSPACE_TIME = SDL_GetTicks();

            // backspace event
            std::cout << "Backspace event\n";
            TextManager::freeKeyboardTypedText->backspaceOrDeleteEvent(false);
        }
    } else if (keyboard[SDL_SCANCODE_DELETE]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            _LAST_BACKSPACE_TIME = SDL_GetTicks();

            // delete event
            std::cout << "Delete event\n";
            TextManager::freeKeyboardTypedText->backspaceOrDeleteEvent(true);
        }
    } else if (keyboard[SDL_SCANCODE_LCTRL] || keyboard[SDL_SCANCODE_RCTRL]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            if (keyboard[SDL_SCANCODE_V]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();

                std::cout << "Paste event\n";
                char *cb = SDL_GetClipboardText();

                TextManager::freeKeyboardTypedText->insert(cb);
                //TextManager::freeKeyboardTypedText->draw();
            } else if (keyboard[SDL_SCANCODE_A]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();

                std::cout << "Select all event\n";
                TextManager::freeKeyboardTypedText->selectAll();

            } else if (keyboard[SDL_SCANCODE_LEFT]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();

                std::cout << "Seek cursor left one word event\n";
                TextManager::freeKeyboardTypedText->moveCursorLeftWord();
            } else if (keyboard[SDL_SCANCODE_RIGHT]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();
                
                std::cout << "Seek cursor right one word event\n";
                TextManager::freeKeyboardTypedText->moveCursorRightWord();
            }
        }
    } else if (keyboard[SDL_SCANCODE_LSHIFT] || keyboard[SDL_SCANCODE_RSHIFT]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            if (keyboard[SDL_SCANCODE_LEFT]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();

                std::cout << "Select one char left event\n";
                TextManager::freeKeyboardTypedText->selectCharacterLeft();
            } else if (keyboard[SDL_SCANCODE_RIGHT]) {
                _LAST_BACKSPACE_TIME = SDL_GetTicks();
                
                std::cout << "Select one char right event\n";
                TextManager::freeKeyboardTypedText->selectCharacterRight();
            }
        }
    } else if (keyboard[SDL_SCANCODE_LEFT]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            _LAST_BACKSPACE_TIME = SDL_GetTicks();

            std::cout << "Move cursor left event\n";
            TextManager::freeKeyboardTypedText->moveCursorLeft();
        }
    } else if (keyboard[SDL_SCANCODE_RIGHT]) {
        if (SDL_GetTicks() >= _LAST_BACKSPACE_TIME + BACKSPACE_REPEAT_TIMER) {
            _LAST_BACKSPACE_TIME = SDL_GetTicks();

            std::cout << "Move cursor right event\n";
            TextManager::freeKeyboardTypedText->moveCursorRight();
        }
    }

    if (Game::event->type == SDL_KEYDOWN && !Game::event->key.repeat) {
        if (Game::event->key.keysym.sym == SDLK_RETURN) {
            
            // FSTREAM IS USED HERE (REMEMBER TO REMOVE)
            std::ofstream f("keyboard_entry.txt");
            for (auto & u : TextManager::freeKeyboardSymbols)
                f << u;
            f.close();
            freeKeyboardText.clear();
            freeKeyboardSymbols.clear();
            freeKeyboardTypedText->clear();
            SDL_StopTextInput();
            return true;
        }

    } else if (Game::event->type == SDL_TEXTINPUT) {

        /*
        unsigned int i = 0u;
        while (Game::event->text.text[i] != '\0') {


            Uint8 c = Game::event->text.text[i];

            if (c == Uint8(0xe2) || c == Uint8(0xc2) || c == Uint8(0xc3) || c == Uint8(0xce) || c == Uint8(0xcf))
                TextManager::freeKeyboardSymbols.emplace_back(TextManager::digestComplexCharacter(Game::event->text.text, i, c));
            else 
                TextManager::freeKeyboardSymbols.emplace_back(c);

            ++i;
        }

        //freeKeyboardText += Game::event->text.text;

        TextManager::destroyFreeKeyboardEntry();
        *TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetNumber, TextManager::freeKeyboardSymbols);
        */
        freeKeyboardTypedText->insert(Game::event->text.text);
        //freeKeyboardTypedText->draw();

    } else if (Game::event->type == SDL_TEXTEDITING) {

        // TODO - I actually don't know what this does
        std::cout << "text edit event\n";
        composition = *Game::event->edit.text;
        cursor = Game::event->edit.start;
        selection_len = Game::event->edit.length;

        //TextManager::destroyFreeKeyboardEntry();
        //*TextManager::freeKeyboardDisplayText = TextManager::addText(x, y, assetNumber, TextManager::freeKeyboardSymbols);
        freeKeyboardTypedText->draw();

    }

    return false;
}

const unsigned int TextManager::widthOfString(const std::string &s, const unsigned int &wLetter) noexcept {
    return (s.size() - (2 * TextManager::count(s, 226))) * wLetter;
}

const unsigned int TextManager::heightOfString(const std::string &s, const unsigned int &hLetter) noexcept {
    return (TextManager::count(s, '\n') + 1) * hLetter;
}

/* Count the amount of times character 'c' occurs in string 's' */
const unsigned int TextManager::count(const std::string &s, const char &c) noexcept {
    unsigned int i = 0u;
    for (auto & sc : s) if (sc == c) ++i;
    return i;
}

/* Destroys all current entities in the free keyboard display text */
void TextManager::destroyFreeKeyboardEntry() {
    for (auto & e : *TextManager::freeKeyboardDisplayText) e->destroy();
    TextManager::freeKeyboardDisplayText->clear();
}