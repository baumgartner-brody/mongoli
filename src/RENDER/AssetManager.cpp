#include "AssetManager.h"

Asset::~Asset() {
    this->_name.clear();
    SDL_FreeTexture(this->_t);
}

AssetManager::~AssetManager() {
    for (auto & a : this->_assets)
        delete a.second;
    this->_assets.clear();
}