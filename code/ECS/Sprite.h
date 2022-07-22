#ifndef _SPRITE_COMPONENT_H_
#define _SPRITE_COMPONENT_H_

#include "SDL2/SDL.h"

#include "ECS.h"
#include "Transform.h"
#include "../render/TextureManager.h"
#include "../render/AssetManager.h"

#include <iostream>

class SpriteComponent : public Component {
public:
    // unused 
    SpriteComponent(const std::string &fileName, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h);
    SpriteComponent(Asset *asset, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h);

    /* Creates a SpriteComponent from an assetName and a source rect. */
    SpriteComponent(const std::string &assetName, const SDL_Rect &srcR);

    /* SpriteComponent copy constructor */
    SpriteComponent(const SpriteComponent &old);

    /* SpriteComponent operator= */
    const SpriteComponent& operator=(const SpriteComponent &old);

    ~SpriteComponent();

    void init() override;

    void draw() override;

    void update() override;

    void copy(Entity &dst, const Entity &old) override;

    /* Equality operators */
    friend const bool operator==(const SpriteComponent &s1, const SpriteComponent &s2) noexcept;
    friend const bool operator!=(const SpriteComponent &s1, const SpriteComponent &s2) noexcept;

    const bool equals(const Entity &e) override { return (*this == e.getComponent<SpriteComponent>()); }

private:

    Asset *_asset;

    TransformComponent *_transform;

    /* The rect of the source texture that will be drawn */
    /*  TransformComponent will dictate dstR */
    SDL_Rect srcR;

    /* Backend deep copy work */
    void _copy(const SpriteComponent &old);
};

#endif /* _SPRITE_COMPONENT_H_ */