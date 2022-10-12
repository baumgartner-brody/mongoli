#ifndef _TEXT_MANAGER_H_
#define _TEXT_MANAGER_H_

#include "../../game.h"
#include "../../ANSI/ANSI.h"
#include "../../render/AssetManager.h"
#include "../../render/TextureManager.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#define FONT_TEXTURE_FILE "src/assets/rexpaint_cp437_10x10.png"

/* The time it takes (in ms) for a backspace event to re-occur */
/* TODO(?) - Eventually going to use a longer timer for determining if repeatable events should happen */
/*  Initial keypress triggers an immediate event, then a ~500 ms timer must be complete before they can repeat */
#define BACKSPACE_REPEAT_TIMER 150

extern std::string freeKeyboardText;
extern std::string composition;
extern Sint32 cursor;
extern Sint32 selection_len;

// TODO - Improve text centering methods (The code in options.cpp is congested)

class Entity;
class TypedText;

/* The TextManager acts more like a helper than a class */
namespace TextManager {

    /* Initializes all text-related assets */
    void init();

    /* Frees all associated memory */
    void free();

    /* Creates the appropriate SDL_Rect for an 8-bit char unsigned int */
    SDL_Rect createSourceRect(const Uint8 &c);

    /* Adds text at the current TextManager position with the desired asset name */
    /* The resultant letter entities are returned */
    std::vector<Entity*> addText(int x, int y, const Uint8 &assetNumber, const std::string &text);

    /* Overload of addText with std::vector<Uint8> to support extended ascii characters */
    std::vector<Entity*> addText(int x, int y, const Uint8 &assetNumber, const std::vector<Uint8> &symbols);

    /* Adds the specified group of extended ascii characters at the specified position */
    /* This method is specifically fit to TypedText's needs */
    std::vector<Entity*> addTextWithHitboxes(int x, int y, const Uint8 &assetNumber, const std::vector<Uint8> &symbols);

    /* Adds a singular character entity at the desired location */
    Entity* addText(int x, int y, const Uint8 &assetNumber, const Uint8 &c);

    // The itr may be moved 
    const Uint8 digestComplexCharacter(const std::string &text, std::string::const_iterator &itr, const Uint8 &first);

    /* Wrapper of digestComplexCharacter with index instead of itr */
    const Uint8 digestComplexCharacter(const char *text, unsigned int &index, const Uint8 &first);

    const Uint8 _d1(const Uint8 &first, const Uint8 &c1);
    const Uint8 _d2(const Uint8 &c1, const Uint8 &c2);

    /* Given a vector of letter entities, iterate thru and recolor them. */
    void recolorText(std::vector<Entity*> &text_to_recolor, const Uint8 &newAssetColorNumber);

    /* Generates a random foreground color to go with the provided background color */
    const Uint8 generateRandomTextColor(const Uint8 &background_color);

    /* Sets the CAPS bool to the current state of CAPSLOCK */
    /* This should be called once before every attempt to get text from the player */
    void getCAPSState();

    /* Allows the user to type until an [ENTER] key is detected */
    const bool freeKeyboardEnter(int x, int y, const Uint8 &assetNumber);

    const bool freeKeyboardEnter2(int x, int y, const Uint8 &assetNumber);

    const unsigned int widthOfString(const std::string &s, const unsigned int &wLetter) noexcept;
    const unsigned int heightOfString(const std::string &s, const unsigned int &hLetter) noexcept;

    /* Count the amount of times character 'c' occurs in string 's' */
    const unsigned int count(const std::string &s, const char &c) noexcept;

    /* Destroys all current entities in the free keyboard display text */
    void destroyFreeKeyboardEntry();

    extern int TEXT_POSITION_X;
    extern int TEXT_POSITION_Y;

    extern std::string *freeKeyboardEntry;
    extern std::vector<Entity*> *freeKeyboardDisplayText;

    extern std::vector<Uint8> freeKeyboardSymbols;

    extern bool *CAPS;

    extern TypedText *freeKeyboardTypedText;
};

#endif /* _TEXT_MANAGER_H_ */