#include "Sprite.h"

/* Definitions for SpriteComponent.h */

/* SpriteComponent value constructor */
SpriteComponent::SpriteComponent(Asset *asset, const int &xpos, const int &ypos, const unsigned int &w, const unsigned int &h) {

    this->_asset = asset;

    this->srcR.x = xpos; this->srcR.y = ypos;
    this->srcR.w = w; this->srcR.h = h;
}

/* Creates a SpriteComponent from an assetName and a source rect. */
SpriteComponent::SpriteComponent(const std::string &assetName, const SDL_Rect &srcR) {
    this->_asset = Game::assetManager->getAsset(assetName);
    this->srcR = srcR;
}


/* There is no dynamic memory managed by SpriteComponent */
SpriteComponent::~SpriteComponent() {}

/* SpriteComponent copy constructor */
SpriteComponent::SpriteComponent(const SpriteComponent &old) {
    this->_copy(old);
}

/* SpriteComponent operator= */
const SpriteComponent& SpriteComponent::operator=(const SpriteComponent &old) {
    /* Guard self assignment */
    if (*this == old) return *this;

    /* "Clear" this and create a copy of old */
    this->_copy(old);
    return *this;
}

void SpriteComponent::init() {
    if (!this->entity->hasComponent<TransformComponent>()) {
        std::cerr << "Tried to create a SpriteComponent on an entity with no transform\n";
        exit(-1);
    }
    this->_transform = &this->entity->getComponent<TransformComponent>();
}

void SpriteComponent::draw() {
    /* TODO: May not be efficient to make a SDL_Rect every draw iteration. */
    SDL_Rect dstR = { _transform->xpos, _transform->ypos, _transform->width, _transform->height };

    TextureManager::draw(this->_asset->getTexture(), this->srcR, dstR);
}

void SpriteComponent::update() {}

/* Overwritten copy method */
void SpriteComponent::copy(Entity &dst, const Entity &old) {
    dst.addComponent<SpriteComponent>(old.getComponent<SpriteComponent>());
}

/* Operator== for SpriteComponents will only check if the _assets and _transforms point to the */
/*  same place. */
const bool operator==(const SpriteComponent &s1, const SpriteComponent &s2) noexcept {
    return (s1.srcR == s2.srcR) && (s1._asset == s2._asset) && (s1._transform == s2._transform);
}

const bool operator!=(const SpriteComponent &s1, const SpriteComponent &s2) noexcept { return !(s1 == s2); }

/* Backend deep copy creation for both copy constructor and assignment operator */
void SpriteComponent::_copy(const SpriteComponent &old) {
    this->_asset = old._asset;
    this->srcR = old.srcR;
}