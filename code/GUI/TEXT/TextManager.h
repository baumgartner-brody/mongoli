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

class Entity;

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
    std::vector<Entity*> addText(int x, int y, const std::string &assetName, const std::string &text);

    /* Adds a singular character entity at the desired location */
    Entity* addText(int x, int y, const std::string &assetName, const Uint8 &c);

    /* Given a vector of letter entities, iterate thru and recolor them. */
    void recolorText(std::vector<Entity*> &text_to_recolor, const std::string &newColorAssetName);

    /* Generates a random foreground color to go with the provided background color */
    const std::string generateRandomTextColor(const std::string &BG_COLOR);

    /* Sets the CAPS bool to the current state of CAPSLOCK */
    /* This should be called once before every attempt to get text from the player */
    void getCAPSState();

    /* Allows the user to type until an [ENTER] key is detected */
    const bool freeKeyboardEnter(int x, int y, const std::string &assetName);

    /* Destroys all current entities in the free keyboard display text */
    void destroyFreeKeyboardEntry();

    extern int TEXT_POSITION_X;
    extern int TEXT_POSITION_Y;

    extern std::string *freeKeyboardEntry;
    extern std::vector<Entity*> *freeKeyboardDisplayText;

    extern bool *CAPS;
};

#endif /* _TEXT_MANAGER_H_ */