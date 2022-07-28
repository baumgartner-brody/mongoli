#include "AssetManager.h"

Asset::Asset(SDL_Texture *t) {
    this->_texture = t;
}

/* Copy constructor for asset, (just a shallow copy of the Texture*) */
Asset::Asset(const Asset &old) {
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
    this->_texture = nullptr;
}

AssetManager::AssetManager() {

}

AssetManager::~AssetManager() { this->clear(); }

Asset* AssetManager::getAsset(const std::string &assetName) {
    return this->assets[assetName];
}

void AssetManager::addAsset(const std::string &assetName, SDL_Texture *t) {
    /* Prevent duplicate assets (and mem leaks) */
    if (this->assets.count(assetName) == 0)
        this->assets[assetName] = new Asset(t);
}

void AssetManager::clear() noexcept {
    for (auto & a : assets)
        delete a.second;
    assets.clear();
}