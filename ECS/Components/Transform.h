#ifndef _TRANSFORM_H_
#define _TRANSFORM_H_

#include "SDL2/SDL.h"

#include "../Components.h"

class Transform : public Component {
public:
    Transform(const int &x, const int &y, const int &w, const int &h) : _r({ x, y, w, h }) {}
    Transform(const SDL_Rect &r) : _r(r) {}
    Transform(const Transform &old) : _r(old._r) {}
    ~Transform() = default;

    void copy(Entity &dst, const Entity &src) override {
        dst.addComponent<Transform>(src.getComponent<Transform>());
    }

    const bool equals(const Entity &e) override {
        return (*this == e.getComponent<Transform>());
    }

    friend const bool operator==(const Transform &t1, const Transform &t2) noexcept {
        return (t1._r.x == t2._r.x) && (t1._r.y == t1._r.y) && (t1._r.w == t2._r.w) && (t1._r.h == t1._r.h);
    }
    friend const bool operator!=(const Transform &t1, const Transform &t2) noexcept { return !(t1 == t2); }

    SDL_Rect _r;
    
};

#endif /* _TRANSFORM_H_ */