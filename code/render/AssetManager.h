#ifndef _ASSETMANAGER_H_
#define _ASSETMANAGER_H_

#include "SDL2/SDL.h"

#include <string>
#include <map>

/* Forward declaration of AssetManager so it can be Asset's friend */
class AssetManager;

class Asset {
public:

    SDL_Texture* getTexture() const { return this->_texture; }

    friend class AssetManager;
private:
    /* Assets require a Texture* to be created */
    Asset(SDL_Texture *t);

    /* Copy constructor for asset, (just a shallow copy of the Texture*) */
    Asset(const Asset &old);

    /* Copy assignment for asset, (just a shallow copy of the Texture*) */
    const Asset& operator=(const Asset &old);

    /* Frees the _texture */
    ~Asset();

    SDL_Texture *_texture = nullptr;
};

class AssetManager {
public:

    AssetManager();
    ~AssetManager();

    /* getAsset - used for both adding and getting assets. */
    /*  getAsset with no SDL_Texture* should only be used when you are certain the texture exists within */
    /*  the asset manager. It will exit the program otherwise. */
    Asset* getAsset(const std::string &assetName);

    /* Could be used to modify an asset but is mainly used for adding */
    void addAsset(const std::string &assetName, SDL_Texture *t);

    void clear();
private:
    std::map<const std::string, Asset*> assets;
};

#endif /* _ASSETMANAGER_H_ */