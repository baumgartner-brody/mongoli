#ifndef _SPRITE_COMPONENT_H_
#define _SPRITE_COMPONENT_H_

#include "SDL2/SDL.h"

#include "ECS.h"
#include "Transform.h"
#include "Animation.h"
#include "../render/TextureManager.h"
#include "../render/AssetManager.h"

#include <iostream>

class SpriteComponent : public Component {
public:
    // unused 
    //SpriteComponent(const std::string &fileName, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h);
    SpriteComponent(Asset *asset, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h);

    /* Creates a SpriteComponent from an assetName and a source rect. */
    SpriteComponent(const Uint8 &assetNumber, const SDL_Rect &srcR);

    /* Creates a SpriteComponent from an assetName and source rect dimensions. */
    SpriteComponent(const Uint8 &assetNumber, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h);

    /* SpriteComponent copy constructor */
    SpriteComponent(const SpriteComponent &old);

    /* SpriteComponent operator= */
    const SpriteComponent& operator=(const SpriteComponent &old);

    ~SpriteComponent();

    void init() override;

    void draw() override;

    void update() override;

    void copy(Entity &dst, const Entity &old) override;

    void addAnimation(const uint8_t &index, const uint8_t &num_frames, const unsigned int &speed, const unsigned int &y);

    /* Swap this Sprite's asset* with a new one */
    void swapAsset(const Uint8 &assetNumber);

    /* Adjust this Sprite's srcRect */
    void moveSrcR(const int &xpos, const int &ypos) noexcept { this->srcR.x = xpos; this->srcR.y = ypos; }
    void adjustSrcR(const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h) noexcept {
        this->srcR.x = xpos; this->srcR.y = ypos;
        this->srcR.w = w; this->srcR.h = h;
    }

    /* Equality operators */
    friend const bool operator==(const SpriteComponent &s1, const SpriteComponent &s2) noexcept;
    friend const bool operator!=(const SpriteComponent &s1, const SpriteComponent &s2) noexcept;

    const bool equals(const Entity &e) override { return (*this == e.getComponent<SpriteComponent>()); }

    void play(const Uint8 &index);

    const bool playingAnimation() const { return this->_current_animation != nullptr; }

    const bool hasAnimation(const Uint8 &index) const { return this->_animations.count(index) != 0u; }

    const bool hasAnimations() const { return !this->_animations.empty(); }

private:

    Asset *_asset;

    TransformComponent *_transform;

    /* The rect of the source texture that will be drawn */
    /*  TransformComponent will dictate dstR */
    SDL_Rect srcR;

    /* Backend deep copy work */
    void _copy(const SpriteComponent &old);

    std::map<const Uint8, Animation*> _animations;

    Animation *_current_animation = nullptr;
};

#endif /* _SPRITE_COMPONENT_H_ */