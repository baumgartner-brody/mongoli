#include "TextureManager.h"

/* Attempts to create an SDL_Surface from "filename." */
SDL_Surface* TextureManager::SurfaceTools::createSurface(const std::string &fileName) {

    SDL_Surface *s = IMG_Load(fileName.c_str());

    /* If this surface could not be created, inform the user and exit */
    if (!s) {
        std::cerr << "Could not create surface from \"" << fileName << "\"\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1); 
    }

    return s;

}

/* Attempts to create a 32-bit RGBA surface with the specified dimensions */
SDL_Surface* TextureManager::SurfaceTools::createSurfaceWithDimensions(const unsigned int &w, const unsigned int &h) {

    /* Attempt to create a surface */
    SDL_Surface *s = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);

    /* If this surface could not be created, inform the user and exit */
    if (!s) {
        std::cerr << "Could not create surface with dimensions: " << w << ' ' << h << '\n';
        std::cerr << SDL_GetError() << '\n';
        exit(-1); 
    }

    return s;
}

/* Creates and returns a copy of s */
SDL_Surface* TextureManager::SurfaceTools::copySurface(SDL_Surface *s) {

    SDL_Surface *cpy = SDL_ConvertSurface(s, s->format, s->flags);

    /* If the copy was unsuccessful, inform the user then exit */
    if (!cpy) {
        std::cerr << "There was an error copying an SDL_Surface\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1);
    }

    return cpy;
}

/* Converts s into its 32-bit equivalent */
SDL_Surface* TextureManager::SurfaceTools::create32bitEquivalent(const SDL_Surface *s) {

    /* Copy the entirety of s onto a destination Surface* with 32bit depth */
    SDL_Rect r = { 0, 0, s->w, s->h };
    SDL_Surface *sub(TextureManager::SurfaceTools::createSubSurface(s, r));
    return sub;
}

/* Create a subsurface that encapsulates SDL_Rect "r" and returns it's surface */
/*  If r is not within the bounds of "s," the program will exit */
SDL_Surface* TextureManager::SurfaceTools::createSubSurface(const SDL_Surface *s, const SDL_Rect &r) {
    /* Check x bound */
    if (r.x + r.w > s->w) {
        std::cerr << "The specified sub_surface x exceeds the bounds of \"s\"\n";
        std::cerr << "    " << (r.x + r.w) << " > " << s->w << '\n';
        exit(-1);
    }

    /* Check y bound */
    if (r.y + r.h > s->h) {
        std::cerr << "The specified sub_surface y exceeds the bounds of \"s\"\n";
        std::cerr << "    " << (r.y + r.h) << " > " << s->h << '\n';
        exit(-1);
    }

    /* Attempt to create a subSurface */
    SDL_Surface *sub(TextureManager::SurfaceTools::createSurfaceWithDimensions(r.w, r.h));

    /* Attempt to blit the specified section of s into sub */
    if (SDL_BlitSurface(const_cast<SDL_Surface*>(s), &r, sub, NULL) != 0) {
        std::cerr << "There was an error creating a subsurface\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1);
    }

    return sub;
}

/* Iterates thru the pixels in s and replaces every instance of c_in with c_out */
void TextureManager::SurfaceTools::recolorSurface(const SDL_Color &c_in, const SDL_Color &c_out, SDL_Surface *s) {
    
    Uint32 *pixels = (Uint32*)s->pixels;

    /* Store the colors as Uint32 so we don't have to perform the operation every iteration */
    Uint32 color_in = SDL_MapRGBA(s->format, c_in.r, c_in.g, c_in.b, c_in.a);
    Uint32 color_out = SDL_MapRGBA(s->format, c_out.r, c_out.g, c_out.b, c_out.a);

    for (unsigned int i = 0u; i < (s->w * s->h); ++i) 
        if (pixels[i] == color_in)
            pixels[i] = color_out;
}
          
/* Uses src to evaluate which pixels should be changed and applies the actual changes to dst */
void TextureManager::SurfaceTools::recolorDstSurface(const SDL_Color &c_in, const SDL_Color &c_out, const SDL_Surface *src, SDL_Surface *dst) {

    const Uint32 *srcPixels = (Uint32*)src->pixels;
    Uint32 *dstPixels = (Uint32*)dst->pixels;

    if (!srcPixels) {
        std::cerr << "Could not get srcPixels\n";
        exit(-1);
    }
    if (!dstPixels) {
        std::cerr << "Could not get dstPixels\n";
        exit(-1);
    }

    /* Store the colors as Uint32 so we don't have to perform the operation every iteration */
    Uint32 color_in = SDL_MapRGBA(src->format, c_in.r, c_in.g, c_in.b, c_in.a);
    Uint32 color_out = SDL_MapRGBA(src->format, c_out.r, c_out.g, c_out.b, c_out.a);

    for (Uint32 i = 0u; i < (src->w * src->h); ++i)
        if (srcPixels[i] == color_in)
            dstPixels[i] = color_out;
}

/* Creates an SDL_Texture* from an SDL_Surface* and then frees it */
SDL_Texture* TextureManager::TextureTools::createTexture(SDL_Surface *s) {
    SDL_Texture *t(SDL_CreateTextureFromSurface(Game::renderer, s));
    SDL_FreeSurface(s);

    /* If this texture could not be created, inform the user and exit */
    if (!t) {
        std::cerr << "Could not create texture from surface\n";
        std::cerr << SDL_GetError() << '\n';
        exit(-1); 
    }

    return t;
}

/* Creates an SDL_Texture* directly from a string fileName */
SDL_Texture* TextureManager::TextureTools::createTexture(const std::string &fileName) {

    SDL_Surface *s(SurfaceTools::createSurface(fileName));
    SDL_Texture *t(TextureManager::TextureTools::createTexture(s));

    return t;
}

/* Creates an SDL_Texture* from fileName, where [0] c_in --> [0] c_out */
SDL_Texture* TextureManager::TextureTools::createRecoloredTexture(const std::string &fileName, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out) {

    /* Check if c_in.size() == c_out.size() */
    /*  Exit if this fails */
    if (c_in.size() != c_out.size()) {
        std::cerr << "Size of c_in vector did not match size of c_out vector!\n";
        exit(-1);
    }

    /* Create a surface from the fileName IMG */
    SDL_Surface *src(TextureManager::SurfaceTools::createSurface(fileName));

    /* Convert any non 32-bit surfaces to 32-bit surfaces */
    if (src->format->BytesPerPixel != 4) {
        SDL_Surface *sub(TextureManager::SurfaceTools::create32bitEquivalent(src));
        SDL_FreeSurface(src);
        src = sub;
    }

    /* If only one color swap needs to be performed, this can be optimized by not */
    /*  making a copy and just calling TextureManager::recolorSurface() */
    if (c_in.size() == 1) {
        TextureManager::SurfaceTools::recolorSurface(c_in[0], c_out[0], src);
        return TextureManager::TextureTools::createTexture(src);
    }

    /* Create a copy of the original surface */
    SDL_Surface *cpy(TextureManager::SurfaceTools::copySurface(src));

    /* Perform the color swap */
    for (unsigned int i = 0u; i < c_in.size(); ++i)
        TextureManager::SurfaceTools::recolorDstSurface(c_in[i], c_out[i], src, cpy);
    
    /* Create the Texture* and free the two surfaces */
    SDL_Texture *t(SDL_CreateTextureFromSurface(Game::renderer, cpy));
    SDL_FreeSurface(src);
    SDL_FreeSurface(cpy);

    return t;

}

/* Draws a rectangle with the specifications and color to the game renderer */
void TextureManager::drawRect(const SDL_Rect &r, const SDL_Color &c) {
    SDL_Color tmp;
    SDL_GetRenderDrawColor(Game::renderer, &tmp.r, &tmp.g, &tmp.b, &tmp.a);
    SDL_SetRenderDrawColor(Game::renderer, c.r, c.g, c.b, c.a);
    SDL_RenderFillRect(Game::renderer, &r); // SDL_RenderDrawRect will draw the outline of the rect 
    SDL_SetRenderDrawColor(Game::renderer, tmp.r, tmp.g, tmp.g, tmp.a);
}

void TextureManager::draw(SDL_Texture *t, SDL_Rect &src, SDL_Rect &dst) {
    SDL_RenderCopy(Game::renderer, t, &src, &dst);
}

/* Equality operators for SDL_Colors */
const bool operator==(const SDL_Color &c1, const SDL_Color &c2) noexcept {
    return (c1.r == c2.r) && (c1.g == c2.g) && (c1.b == c2.b) && (c1.a == c2.a);
}

const bool operator!=(const SDL_Color &c1, const SDL_Color &c2) noexcept { return !(c1 == c2); }