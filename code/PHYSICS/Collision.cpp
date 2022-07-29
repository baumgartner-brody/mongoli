#include "Collision.h"

#include "../ECS/Hitbox.h"

const bool PHYSICS::CollisionAABB(const HitboxComponent &h1, const HitboxComponent &h2) noexcept {
    return (h1._r.x < h2._r.x + h2._r.w) && 
        (h1._r.x + h1._r.w > h2._r.x) && 
        (h1._r.y < h2._r.y + h2._r.h) && 
        (h1._r.y + h1._r.h > h2._r.y);
}