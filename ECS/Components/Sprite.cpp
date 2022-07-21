#include "Sprite.h"

#include "../../RENDER/AssetManager.h"
#include "../../RENDER/TextureManager.h"

Sprite::Sprite(const std::string &asset_name, const int &srcR_x, const int &srcR_y, const int &srcR_w, const int &srcR_h) {
    
    this->_texture = Game::_assetManager->getAsset(asset_name);

    this->_srcR.x = srcR_x;
    this->_srcR.y = srcR_y;
    this->_srcR.w = srcR_w;
    this->_srcR.h = srcR_h;

}

Sprite::Sprite(const std::string &asset_name, const SDL_Rect &srcR) {

    this->_texture = Game::_assetManager->getAsset(asset_name);
    this->_srcR = srcR;

}

Sprite::Sprite(const Sprite &old) {
    this->_texture = old._texture;
    this->_transform = old._transform;
    this->_srcR = old._srcR;
}

Sprite::~Sprite() {}

void Sprite::setSrcR(const int &x, const int &y) noexcept {
    this->_srcR.x = x;
    this->_srcR.y = y;
}

void Sprite::setSrcR(const int &x, const int &y, const int &w, const int &h) noexcept {
    this->_srcR.x = x;
    this->_srcR.y = y;
    this->_srcR.w = w;
    this->_srcR.h = h;
}

void Sprite::init() {
    if (this->entity->hasComponent<Transform>()) {
        this->_transform = &this->entity->getComponent<Transform>();
    } else {
        std::cerr << "Tried to create a sprite without a Transform component!\n"; 
        exit(-1);
    }
}

void Sprite::draw() {
    TextureManager::Texture::draw(this->_texture, this->_srcR, this->_transform->_r);
}