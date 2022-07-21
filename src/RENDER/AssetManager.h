#ifndef _ASSET_MANAGER_H_
#define _ASSET_MANAGER_H_

#include "SDL2/SDL.h"

#include <map>
#include <string>

class Asset {
public:

    Asset(const std::string &name, const SDL_Texture *t) : _name(name), _t(t) {}
    ~Asset();

    /* Read-only reference to this asset's name */
    const std::string name() const { return this->_name; }

private:
    std::string _name;
    SDL_Texture *_t = nullptr;
};

class AssetManager {
public:
    AssetManager() = default;
    ~AssetManager();
private:    
    std::map<const std::string, Asset*> _assets;
};

#endif /* _ASSET_MANAGER_H_ */