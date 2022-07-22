#ifndef _TEXTUREMANAGER_H_
#define _TEXTUREMANAGER_H_

#include "../game.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <iostream>
#include <string>
#include <vector>

/* Equality operators for SDL_Colors */
const bool operator==(const SDL_Color &c1, const SDL_Color &c2) noexcept;
const bool operator!=(const SDL_Color &c1, const SDL_Color &c2) noexcept;

/* The textureManager functions more like a namespace than an actual class. */
/*  It contains several methods for creating/recoloring/combining/ modifying */
/*  SDL_Surfaces and SDL_Textures, but is used via TextureManager::method(); */
/* For most creation methods, the program will exit upon failure. */
namespace TextureManager {

    /* Defines all surface-relevant functions that the TextureManager might use */
    namespace SurfaceTools {
        /* Attempts to create an SDL_Surface from "filename." */
        SDL_Surface* createSurface(const std::string &fileName);

        /* Attempts to create a 32-bit RGBA surface with the specified dimensions */
        SDL_Surface* createSurfaceWithDimensions(const unsigned int &w, const unsigned int &h);

        /* Creates and returns a copy of s */
        SDL_Surface* copySurface(SDL_Surface *s);

        /* Creates a 32-bit equivalent of s and returns it */
        SDL_Surface* create32bitEquivalent(const SDL_Surface *s);

        /* Create a subsurface that encapsulates SDL_Rect "r" and returns it's surface */
        /*  If r is not within the bounds of "s," the program will exit */
        SDL_Surface* createSubSurface(const SDL_Surface *s, const SDL_Rect &r);

        /* Iterates thru the pixels in s and replaces every instance of c_in with c_out */
        void recolorSurface(const SDL_Color &c_in, const SDL_Color &c_out, SDL_Surface *s);

        /* Uses src to evaluate which pixels should be changed and applies the actual changes to dst */
        void recolorDstSurface(const SDL_Color &c_in, const SDL_Color &c_out, const SDL_Surface *src, SDL_Surface *dst);
    };

    /* Defines all texture-relevant functions that the TextureManager might use */
    namespace TextureTools { 
        /* Creates a texture from s and then frees it */
        SDL_Texture* createTexture(SDL_Surface *s);

        /* Attempts to create an SDL_Texture from "filename." */
        SDL_Texture* createTexture(const std::string &fileName);

        /* Creates an SDL_Texture* from fileName, where [0] c_in --> [0] c_out */
        SDL_Texture* createRecoloredTexture(const std::string &fileName, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out);
    };

    /* Draws SDL_Texture *t to the game renderer */
    void draw(SDL_Texture *t, SDL_Rect &src, SDL_Rect &dst);

};

#endif /* _TEXTUREMANAGER_H_ */