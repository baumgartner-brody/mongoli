#ifndef _HITBOX_H_
#define _HITBOX_H_

#include "ECS.h"
#include "Transform.h"

/* HitboxComponent - maintains a hitbox that may or may not be associated with a moving transform component. */
/*  The transform component must be present within the entity upon the hitbox's initiatilzation. */
/*  If a hitbox has a _transform component, its x and y position will follow it. */

class HitboxComponent : public Component {
public:

    HitboxComponent(const SDL_Rect &r) : _r(r) {}
    HitboxComponent(const int &x, const int &y, const int &w, const int &h);
    HitboxComponent(const HitboxComponent &old) { this->_copy(old); }
    ~HitboxComponent();

    /* Operator= for HitboxComponent */
    const HitboxComponent& operator=(const HitboxComponent &old);

    /* Return whether or not this hitbox's _transform points somewhere */
    const bool hasTransform() const { return this->_transform != nullptr; }

    void init() override;

    void draw() override {}

    void update() override;

    void copy(Entity &dst, const Entity &old) override;

    friend const bool operator==(const HitboxComponent &h1, const HitboxComponent &h2) noexcept;
    friend const bool operator!=(const HitboxComponent &h1, const HitboxComponent &h2) noexcept;

    const bool equals(const Entity &e) override { return (*this == e.getComponent<HitboxComponent>()); } 

    /* The SDL_Rect that encompasses this hitbox */
    SDL_Rect _r; 

private:
    /* A hitbox either has a transform component upon creation or does not. */
    TransformComponent *_transform = nullptr;

    /* Copy contents of h into *this */
    void _copy(const HitboxComponent &old) noexcept;
};

#endif /* _HITBOX_H_ */