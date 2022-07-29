#include "Mouse.h"

#include "Hitbox.h"
#include "../game.h"

MouseComponent::MouseComponent(const unsigned int &width, const unsigned int &height) {
    this->_tmp_width_ = new unsigned int(width);
    this->_tmp_height_ = new unsigned int(height);
}

MouseComponent::~MouseComponent() {
    this->_event = nullptr;
    this->_transform = nullptr;
}

void MouseComponent::init() {
    if (this->entity->hasComponent<HitboxComponent>()) {
        this->_hitbox = &this->entity->getComponent<HitboxComponent>();
    } else {
        if (this->_tmp_height_ == nullptr || this->_tmp_width_ == nullptr) {
            std::cerr << "Could not create mouse component due to lack of dimensions.\n";
            exit(-1);
        } else {
            this->_hitbox = &(this->entity->addComponent<HitboxComponent>(0, 0, *this->_tmp_width_, *this->_tmp_height_));
        }   
    }


    this->_event = Game::event;

    /* Free tmp variables */
    if (this->_tmp_height_ != nullptr) {
        delete this->_tmp_height_;
        this->_tmp_height_ = nullptr;
    }
    if (this->_tmp_width_ != nullptr) {
        delete this->_tmp_width_;
        this->_tmp_width_ = nullptr;
    }
}

void MouseComponent::update() {
    if (this->_event->type == SDL_MOUSEMOTION) {
        this->_hitbox->_r.x = this->_event->motion.x;
        this->_hitbox->_r.y = this->_event->motion.y;
    }
}