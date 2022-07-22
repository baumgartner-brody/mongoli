#include "Hitbox.h"

HitboxComponent::HitboxComponent(const int &x, const int &y, const int &w, const int &h) {
    this->_r.x = x;
    this->_r.y = y;
    this->_r.w = w;
    this->_r.h = h;
}

/* Invalidate the _transform * (safe even if it is not had) */
HitboxComponent::~HitboxComponent() {
    this->_transform = nullptr;
}

const HitboxComponent& HitboxComponent::operator=(const HitboxComponent &old) {
    /* Guard against self assignment */
    if (*this == old) return *this;

    /* Copy old and return *this */
    this->_copy(old);
    return *this;
}

/* Set this hitbox's _transform (if applicable) */
void HitboxComponent::init() {
    if (this->entity->hasComponent<TransformComponent>())
        this->_transform = &this->entity->getComponent<TransformComponent>();
}   

/* If this hitbox is attached to a transform, its x and y position will match. */
void HitboxComponent::update() {
    if (this->hasTransform()) {
        this->_r.x = this->_transform->xpos;
        this->_r.y = this->_transform->ypos;
    }
}

/* Equality operator for hitboxes */
const bool operator==(const HitboxComponent &h1, const HitboxComponent &h2) noexcept {
    return (h1._transform == h2._transform) && (h1._r == h2._r);
}

/* Overwritten copy method */
void HitboxComponent::copy(Entity &dst, const Entity &old) {

    dst.addComponent<HitboxComponent>(old.getComponent<HitboxComponent>());
}

/* Backend deep copy creation for both copy constructor and assignment operator */
void HitboxComponent::_copy(const HitboxComponent &old) noexcept {
    this->_transform = old._transform;
    this->_r = old._r;
}