#include "AssetManager.h"

#include <iostream>

/* Assets require a Texture* to be created */
Asset::Asset(SDL_Texture *t) {
    this->_texture = t;
}

/* Some assets can have a texture and a surface */
Asset::Asset(SDL_Surface *s, SDL_Texture *t) {
    this->_surface = s;
    this->_texture = t;
}

/* Copy constructor for asset, (just a shallow copy of the Texture*) */
Asset::Asset(const Asset &old) {
    this->_surface = old._surface;
    this->_texture = old._texture;
}

/* Copy assignment for asset, (just a shallow copy of the Texture*) */
const Asset& Asset::operator=(const Asset &old) {

    /* Guard self assignment */
    if (this->_texture == old._texture) return *this;

    this->_texture = old._texture;
    return *this;
}

/* Calls SDL_DestroyTexture for the internal _texture */
Asset::~Asset() {
    SDL_DestroyTexture(this->_texture);
    SDL_FreeSurface(this->_surface);
    this->_texture = nullptr;
    this->_surface = nullptr;
}

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() { this->clear(); }

Asset* AssetManager::getAsset(const std::string &assetName) {
    if (this->assets.find(assetName) == std::end(this->assets)) {
        std::cerr << "No asset named \"" << assetName << "\" exists!\n";
        exit(-1);
    }
    return this->assets[assetName];
}

void AssetManager::addAsset(const std::string &assetName, SDL_Texture *t) {
    /* Prevent duplicate assets (and mem leaks) */
    if (this->assets.count(assetName) == 0)
        this->assets[assetName] = new Asset(t);
}

void AssetManager::addAsset(const std::string &assetName, SDL_Surface *s, SDL_Texture *t) {
    /* Prevent duplicate assets (and mem leaks) */
    if (this->assets.count(assetName) == 0)
        this->assets[assetName] = new Asset(s, t);
}

void AssetManager::clear() noexcept {
    for (auto & a : assets)
        delete a.second;
    assets.clear();
}