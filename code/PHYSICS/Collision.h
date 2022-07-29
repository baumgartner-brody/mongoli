#ifndef _COLLISION_H_
#define _COLLISION_H_

/* Forward declaration of HitboxComponent */
class HitboxComponent;

namespace PHYSICS {

    const bool CollisionAABB(const HitboxComponent &h1, const HitboxComponent &h2) noexcept;

};

#endif /* _COLLISION_H_ */