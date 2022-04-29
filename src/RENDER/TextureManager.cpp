#include "TextureManager.h"

#include <iostream>

/* Attempts to create an SDL_Surface* from the input file */
SDL_Surface* TextureManager::createSurface(const std::string &file_name) {

    SDL_Surface *s(IMG_Load(file_name.c_str()));

    if (!s) {
        std::cerr << "An error occured creating a surface from \"" << file_name << "\"\n";
        std::cerr << SDL_GetError();
        exit(-1);
    }

    /* If you've made it here the surface is likely valid */
    return s;

}

/* Attempts to create an SDL_Texture* from the input surface */
SDL_Texture* TextureManager::createTexture(SDL_Surface *s) {

    SDL_Texture *t(SDL_CreateTextureFromSurface(Game::_renderer, s));

    if (!t) {
        std::cerr << "An error occured creating a texture from \"" << s << "\"\n";
        std::cerr << SDL_GetError();
        exit(-1);
    }

    /* If you've made it here the texture is likely valid */
    return t;
}

void TextureManager::draw(SDL_Texture *t, const SDL_Rect &dstR) {

    SDL_RenderCopy(Game::_renderer, t, NULL, &dstR);

}