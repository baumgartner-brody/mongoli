#include "Transform.h"

/* Definitions for TransformComponent.h */

TransformComponent::TransformComponent(const int &xpos, const int &ypos, const int &w, const int &h) {
    this->xpos = xpos;
    this->ypos = ypos;
    this->width = w;
    this->height = h;
}

/* TransformComponent deconstructor */
TransformComponent::~TransformComponent() {}

/* Copy constructor */
TransformComponent::TransformComponent(const TransformComponent &old) {
    this->_copy(old);
}

/* operator= */
const TransformComponent& TransformComponent::operator=(const TransformComponent &old) {
    /* Guard against self-assignment */
    if (*this == old) return *this;

    /* No clear method, make copy and return */
    this->_copy(old);
    return *this;
}

/* Overwritten copy method */
void TransformComponent::copy(Entity &dst, const Entity &old) {
    dst.addComponent<TransformComponent>(old.getComponent<TransformComponent>());
}

/* Checks if all t1's data matches t2's data */
const bool operator==(const TransformComponent &t1, const TransformComponent &t2) noexcept {
    return (t1.xpos == t2.xpos) && (t1.ypos == t2.ypos) && (t1.width == t2.width) && (t1.height == t2.height);
}

const bool operator!=(const TransformComponent &t1, const TransformComponent &t2) noexcept { return !(t1 == t2); }

void TransformComponent::_copy(const TransformComponent &old) {
    this->xpos = old.xpos;
    this->ypos = old.ypos;
    this->width = old.width;
    this->height = old.height;
}