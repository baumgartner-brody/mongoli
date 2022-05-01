#include "TextureManager.h"

#include <iostream>

/* Attempts to create an SDL_Surface* from the input file */
SDL_Surface* TextureManager::Surface::createSurface(const std::string &file_name) {

    SDL_Surface *s = IMG_Load(file_name.c_str());

    if (!s) {
        std::cerr << "An error occured creating a surface from \"" << file_name << "\"\n";
        std::cerr << SDL_GetError();
        exit(-1);
    }

    /* If you've made it here the surface is likely valid */
    return s;

}

/* Creates a sub-surface of 's' that encompasses the region 'r' */
/* All sub-surfaces will be 32-bit. */
SDL_Surface* TextureManager::Surface::createSubSurface(const SDL_Surface *s, const SDL_Rect &r) {

    SDL_Surface *sub_s = SDL_CreateRGBSurface(0, r.w, r.h, 32u, 0u, 0u, 0u, 0u);

    if (SDL_BlitSurface(const_cast<SDL_Surface*>(s), &r, sub_s, NULL) != 0) {
        std::cerr << "An error occured creating a sub-surface:\n";
        std::cerr << SDL_GetError();
        exit(-1);
    }

    return sub_s;

}

/* Creates a copy of the source surface 's'. The copy will be a 32-bit. */
SDL_Surface* TextureManager::Surface::copySurface(const SDL_Surface *s) {

    return TextureManager::Surface::createSubSurface(s, { 0, 0, s->w, s->h });

}

/* Converts s to its equivalent 32 bit format */
/* Can essentially be thought of copySurface() and free original */
void TextureManager::Surface::convertTo32BitSurface(SDL_Surface *&s) {

    SDL_Surface *tmp = TextureManager::Surface::copySurface(s);
    SDL_FreeSurface(s);
    s = tmp;

}

/* Iterates thru 2 surfaces - makes evaluations based on 'eval_s' and makes changes to 'dst_s' */
/* Note: This will not work if the Surfaces are not 32-bit - it will print an error and exit  */
void TextureManager::Surface::recolorSurface(const SDL_Surface *eval_s, SDL_Surface *dst_s, const SDL_Color &c_in, const SDL_Color &c_out) {

    /* Safety */
    if (!TextureManager::Surface::is32bit(eval_s)) {
        std::cerr << "The eval_s is not 32-bit!\n";
        exit(-1);
    }

    if (!TextureManager::Surface::is32bit(dst_s)) {
        std::cerr << "The dst_s is not 32-bit!\n";
        exit(-1);
    }

    Uint32 *eval_pixels = (Uint32*)(eval_s->pixels);
    Uint32 *dst_pixels = (Uint32*)(dst_s->pixels);

    /* Perform these operations once rather than every iteration */
    Uint32 _c_in = SDL_MapRGBA(eval_s->format, c_in.r, c_in.g, c_in.b, c_in.a);
    Uint32 _c_out = SDL_MapRGBA(eval_s->format, c_out.r, c_out.g, c_out.b, c_out.a);

    for (unsigned int i = 0u; i < (eval_s->w * eval_s->h); ++i) {
        /* DEBUG */ /* std::cout << "i: " << i << '\n'; */ 
        if (eval_pixels[i] == _c_in)
            dst_pixels[i] = _c_out;
    }

    dst_s->pixels = dst_pixels;
}

/* Iterates thru a surface and recolors all pixels of color 'c_in' to color 'c_out' */
/* This should be used when you only need to change one color and you don't care about retaining the original's integrity */
/* Note: This will automatically convert 's' to a 32-bit surface if needed. */
void TextureManager::Surface::recolorSurface(SDL_Surface *&s, const SDL_Color &c_in, const SDL_Color &c_out) {

    /* Auto-convert to 32 bit */
    if (!TextureManager::Surface::is32bit(s))
        TextureManager::Surface::convertTo32BitSurface(s);

    TextureManager::Surface::recolorSurface(s, s, c_in, c_out);
}

/* Performs TextureManager::Surface::recolorSurface(eval_s, dst_s), converting every color in c_in to its corresponding c_out color. */
/* Note: This will automatically convert 'eval_s' and 'dst_s' to 32-bit surfaces if needed. */
void TextureManager::Surface::recolorSurface(const SDL_Surface *eval_s, SDL_Surface *dst_s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out) {

    SDL_Surface *eval_s_32b = nullptr;
    
    /* Convert eval_s to 32b if need be */
    if (!TextureManager::Surface::is32bit(eval_s)) 
        eval_s_32b = TextureManager::Surface::copySurface(eval_s);
    
    /* Convert dst_s to 32b if need be */
    if (!TextureManager::Surface::is32bit(dst_s)) 
        TextureManager::Surface::convertTo32BitSurface(dst_s);


    /* Validate vector sizes */
    if (c_in.empty() || c_out.empty() || c_in.size() != c_out.size()) {
        std::cerr << "Recolor vector size mismatch!\n";
        std::cerr << "\tc_in.size(): " << c_in.size() << '\n';
        std::cerr << "\tc_out.size(): " << c_out.size() << '\n';
        exit(-1);
    }

    for (unsigned int i = 0u; i < c_in.size(); ++i)
        TextureManager::Surface::recolorSurface(eval_s_32b, dst_s, c_in[i], c_out[i]);

}

/* Performs TextureManager::Surface::recolorSurface(s, s), essentially removing the need to provide a dst_surface */
/* Note: This will automatically convert 's' to a 32-bit surface if needed. */
void TextureManager::Surface::recolorSurface(SDL_Surface *&s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out) {

    SDL_Surface *dst = TextureManager::Surface::createRecoloredSurface(s, c_in, c_out);
    SDL_FreeSurface(s);
    s = dst;

}

/* Creates a SDL_Surface and performs TextureManager::Surface::recolorSurface(eval_s, dst_s, c_in, c_out) on it */
SDL_Surface* TextureManager::Surface::createRecoloredSurface(const SDL_Surface *eval_s, const std::vector<SDL_Color> &c_in, const std::vector<SDL_Color> &c_out) {
    SDL_Surface *dst = TextureManager::Surface::copySurface(eval_s);
    TextureManager::Surface::recolorSurface(eval_s, dst, c_in, c_out);
    return dst;
}

const bool TextureManager::Surface::is32bit(const SDL_Surface *s) {

    /* Safety */
    if (s == nullptr) {
        std::cerr << "A nullptr was passed into TextureManager::Surface::is32bit()!\n";
        exit(-1);
    }

    return s->format->BitsPerPixel == Uint8(32u);

}

/* Attempts to create an SDL_Texture* from the input surface */
SDL_Texture* TextureManager::Texture::createTexture(SDL_Surface *s) {

    SDL_Texture *t = SDL_CreateTextureFromSurface(Game::_renderer, s);

    if (!t) {
        std::cerr << "An error occured creating a texture from \"" << s << "\"\n";
        std::cerr << SDL_GetError();
        exit(-1);
    }

    /* If you've made it here the texture is likely valid */
    return t;
}

/* Attempts to create an SDL_Texture* directly from an input file */
SDL_Texture* TextureManager::Texture::createTexture(const std::string &file_name) {

    SDL_Surface *s = TextureManager::Surface::createSurface(file_name);
    SDL_Texture *t = TextureManager::Texture::createTexture(s);
    SDL_FreeSurface(s);

    return t;
}

void TextureManager::Texture::draw(SDL_Texture *t, const SDL_Rect &dstR) {

    SDL_RenderCopy(Game::_renderer, t, NULL, &dstR);

}