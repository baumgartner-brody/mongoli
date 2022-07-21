#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "SDL2/SDL.h"

#include <map>
#include <string>

class Asset {
public:

    Asset(SDL_Texture *t) : _t(t) {}
    ~Asset();

    /* Read-only reference to this asset's name */
    SDL_Texture* texture() const { return this->_t; }

private:
    SDL_Texture *_t = nullptr;
};

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();

    SDL_Texture* getAsset(const std::string &asset_name, SDL_Texture *t) noexcept;

    const bool assetExist(const std::string &asset_name) const { return this->_assets.count(asset_name) != 0; }

    void clear() noexcept;

private:    
    std::map<const std::string, Asset*> _assets;
};

#endif /* _ASSET_MANAGER_H_ */