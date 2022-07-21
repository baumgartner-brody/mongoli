#include "Sprite.h"

#include "../../RENDER/AssetManager.h"
#include "../../RENDER/TextureManager.h"

Sprite::Sprite(const std::string &asset_name) {

    this->_asset_name = asset_name;
    
}

Sprite::Sprite(const Sprite &old) {
    this->_asset_name = old._asset_name;
    this->_transform = old._transform;
}

Sprite::~Sprite() {
    this->_asset_name.clear();
}

void Sprite::init() {
    if (this->entity->hasComponent<Transform>()) {
        this->_transform = &this->entity->getComponent<Transform>();
    } else {
        std::cerr << "Tried to create a sprite with an invalid asset name!\n"; 
        exit(-1);
    }
}

void Sprite::draw() {
    TextureManager::Texture::draw(Game::_assetManager->getAsset(this->_asset_name, nullptr), this->_transform->_r);
}