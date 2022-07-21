#ifndef _TEXTURE_MANAGER_H_
#define _TEXTURE_MANAGER_H_

#include "../Game.h"

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"

#include <vector>

namespace TextureManager {

    namespace Surface {

        /* Attempts to create an SDL_Surface* from the input file */
        SDL_Surface* createSurface(const std::string &file_name);

        /* Creates a sub-surface of 's' that encompasses the region 'r' */
        /* All sub-surfaces will be 32-bit. */
        SDL_Surface* createSubSurface(const SDL_Surface *s, const SDL_Rect &r);

        /* Creates a copy of the source surface 's'. The copy will be a 32-bit. */
        /* Note: This will NOT free surface 's' */
        SDL_Surface* copySurface(const SDL_Surface *s);

        /* Converts s to its equivalent 32 bit format */
        /* Can essentially be thought of copySurface() and free original */
        void convertTo32BitSurface(SDL_Surface *&s);

        /* Iterates thru 2 surfaces - makes evaluations based on 'eval_s' and makes changes to 'dst_s' */
        /* Note: This will not work if the Surfaces are not 32-bit - it will print an error and exit  */
        void recolorSurface(const SDL_Surface *eval_s, SDL_Surface *dst_s, const SDL_Color &c_in, const SDL_Color &c_out);

        /* Iterates thru a surface and recolors all pixels of color 'c_in' to color 'c_out' */
        /* This should be used when you only need to change one color and you don't care about retaining the original's integrity */
        /* Note: This will automatically convert 's' to a 32-bit surface if needed. */
        void recolorSurface(SDL_Surface *&s, const SDL_Color &c_in, const SDL_Color &c_out);

        /* Performs TextureManager::Surface::recolorSurface(eval_s, dst_s), converting every color in c_in to its corresponding c_out color. */
        /* Note: This will automatically convert 'eval_s' and 'dst_s' to 32-bit surfaces if needed. */
        void recolorSurface(const SDL_Surface *eval_s, SDL_Surface *dst_s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out);

        /* Performs TextureManager::Surface::recolorSurface(s, s), essentially removing the need to provide a dst_surface */
        /* Note: This will automatically convert 's' to a 32-bit surface if needed. */
        void recolorSurface(SDL_Surface *&s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out);

        /* Creates a SDL_Surface and performs TextureManager::Surface::recolorSurface(eval_s, dst_s, c_in, c_out) on it */
        SDL_Surface* createRecoloredSurface(const SDL_Surface *eval_s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out);

        /* Determines whether or not 's' is a 32-bit surface */
        const bool is32bit(const SDL_Surface *s);

    };

    namespace Texture {

        /* Attempts to create an SDL_Texture* from the input surface */
        SDL_Texture* createTexture(SDL_Surface *s);

        /* Attempts to create an SDL_Texture* directly from an input file */
        SDL_Texture* createTexture(const std::string &file_name);

        /* Draws a texture to the renderer */
        void draw(SDL_Texture *t, const SDL_Rect &dstR);

        /* Draws a texture to the renderer (with additional srcR support) */
        void draw(SDL_Texture *t, const SDL_Rect &srcR, const SDL_Rect &dstR);
    };
};

#endif /* _TEXTURE_MANAGER_H_ */