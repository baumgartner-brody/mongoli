#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "SDL2/SDL.h"

#include <map>
#include <string>

class Asset {
public:

    Asset(SDL_Texture *&t) : _t(t) {}
    ~Asset();
    
    SDL_Texture *_t = nullptr;
};

class AssetManager {
public:

    AssetManager() = default;
    ~AssetManager() { this->clear(); }

    /* Attempts to get a read-write texture* using the name of an asset. */
    /* The program will crash if the asset is non-existent */
    SDL_Texture* getAsset(const std::string &assetname);

    /* Probes the assetmanager to see if a particular asset exists. */
    const bool hasAsset(const std::string &assetname) const noexcept;

    /* Create an asset with a name and a texture */
    /* The program will crash if t == nullptr and a warning will be printed if the asset exists. */
    void createAsset(const std::string &assetname, SDL_Texture *t);

    /* Attempt to delete an asset from the AssetManager */
    /* Print a warning and continue if asset is non-existent */
    void deleteAsset(const std::string &assetname) noexcept;

    /* Clears all assets (and their memory) from the asset manager */
    void clear() noexcept;

private:

    std::map<const std::string, Asset*> _assets;
};

#endif /* _ASSET_MANAGER_H_ */