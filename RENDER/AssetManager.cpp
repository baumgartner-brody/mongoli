#include "AssetManager.h"

#include <iostream>


/* Asset::Asset(SDL_Texture *&) is defined in AssetManager.h */


Asset::~Asset() {
    SDL_DestroyTexture(this->_t);
    this->_t = nullptr;
}

/* Attempts to get a read-write texture* using the name of an asset. */
/* The program will crash if the asset is non-existent */
SDL_Texture* AssetManager::getAsset(const std::string &assetname) {

    /* Validity check */
    if (!this->hasAsset(assetname)) {
        std::cerr << "Asset \"" << assetname << "\" does not exist!\n";
        exit(-1);
    }

    return this->_assets[assetname]->_t;

}

/* Probes the assetmanager to see if a particular asset exists. */
const bool AssetManager::hasAsset(const std::string &assetname) const noexcept {
    return this->_assets.find(assetname) != std::end(this->_assets);
}

/* Create an asset with a name and a texture */
/* The program will crash if t == nullptr and a warning will be printed if the asset exists. */
void AssetManager::createAsset(const std::string &assetname, SDL_Texture *t) {
    
    /* Guard against null assets */
    if (t == nullptr) {
        std::cerr << "Cannot create an asset \"" << assetname << "\" with a null texture!\n";
        exit(-1);
    }

    /* Print a warning and do nothing if asset exist */
    if (this->hasAsset(assetname)) {
        std::cerr << "WARN: Asset \"" << assetname << "\" already exists!\n";
    /* Create the asset */
    } else {
        this->_assets[assetname] = new Asset(t);
    }
} 

/* Attempt to delete an asset from the AssetManager */
/* Print a warning and continue if asset is non-existent */
void AssetManager::deleteAsset(const std::string &assetname) noexcept {
    if (!this->hasAsset(assetname)) {
        std::cerr << "WARN: The asset \"" << assetname << "\" does not exist! (No deletion will be performed).\n";
    } else {
        auto a_itr = this->_assets.find(assetname);
        delete a_itr->second;
        this->_assets.erase(a_itr);
    }
}

/* Clears all assets (and their memory) from the asset manager */
void AssetManager::clear() noexcept {
    for (auto & a : this->_assets)
        delete a.second;
    this->_assets.clear();
}