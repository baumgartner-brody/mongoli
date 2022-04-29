#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../Game.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

namespace TextureManager {

    /* Attempts to create an SDL_Surface* from the input file */
    SDL_Surface* createSurface(const std::string &file_name);

    /* Attempts to create an SDL_Texture* from the input surface */
    SDL_Texture* createTexture(SDL_Surface *s);

    /* Draws a texture to the renderer */
    void draw(SDL_Texture *t, const SDL_Rect &dstR);
};

#endif /* _TEXTURE_MANAGER_H_ */