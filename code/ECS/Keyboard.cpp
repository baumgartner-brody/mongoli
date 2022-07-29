#include "Keyboard.h"

#include "../game.h"

KeyboardController::~KeyboardController() {
    this->_event = nullptr;
    this->_transform = nullptr;
}

/* Attempt to retreive this entity's transform component, exit if it is not found */
void KeyboardController::init() {
    if (!this->entity->hasComponent<TransformComponent>()) {
        std::cerr << "Attempted to add a KeyboardController to an entity with no transform component!\n";
        exit(-1);
    } else {
        this->_transform = &this->entity->getComponent<TransformComponent>();
        this->_event = Game::event;
    }
}

void KeyboardController::update() {

    /* KEYDOWN EVENTS */
    if (this->_event->type == SDL_KEYDOWN && !this->_event->key.repeat) {
        if (this->_event->key.keysym.sym == SDLK_w) {
            this->_transform->ypos -= 10;
        } else if (this->_event->key.keysym.sym == SDLK_a) {
            this->_transform->xpos -= 10;
        } else if (this->_event->key.keysym.sym == SDLK_s) {
            this->_transform->ypos += 10;
        } else if (this->_event->key.keysym.sym == SDLK_d) {
            this->_transform->xpos += 10;
        }

    /* KEYUP EVENTS*/
    } else if (this->_event->type == SDL_KEYUP) {

    }

}

void KeyboardController::_copy(const KeyboardController &old) noexcept {
    this->_transform = old._transform;
    this->_event = old._event;
}