#ifndef _TEXT_MANAGER_H_
#define _TEXT_MANAGER_H_

#include "../game.h"
#include "../ANSI/ANSI.h"
#include "../render/AssetManager.h"
#include "../render/TextureManager.h"

#include <iostream>
#include <string>
#include <vector>

#define FONT_TEXTURE_FILE "src/assets/cp10x10.png"

/* The TextManager acts more like a helper than */
namespace TextManager {

    /* Initializes all text-related assets */
    void init();

    /* Creates the appropriate SDL_Rect for an 8-bit char int */
    SDL_Rect createSourceRect(const Uint8 &c);

    
};

#endif /* _TEXT_MANAGER_H_ */