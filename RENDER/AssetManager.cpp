#include "AssetManager.h"

#include <iostream>

Asset::~Asset() {
    SDL_DestroyTexture(this->_t);
    this->_t = nullptr;
}

AssetManager::~AssetManager() {
    this->clear();
}

SDL_Texture* AssetManager::getAsset(const std::string &asset_name, SDL_Texture *t) noexcept {
    if (!this->assetExist(asset_name)) 
        /* Guard against creating nullptr assets */
        if (t == nullptr) {
            std::cerr << "Cannot create/get an asset with a null texture!\n";
            std::cerr << "\tasset_name: \"" << asset_name << "\"\n";
            exit(-1);
        } else {
            this->_assets[asset_name] = new Asset(t);
        }

    return this->_assets[asset_name]->texture();
}

void AssetManager::clear() noexcept {
    for (auto & a : this->_assets)
        delete a.second;
    this->_assets.clear();
}