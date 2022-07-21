#ifndef _SPRITE_H_
#define _SPRITE_H_

#include "SDL2/SDL.h"

#include "../Components.h"
#include <string>

class Asset;
class Transform;

class Sprite : public Component {
public:
    Sprite(const std::string &asset_name);
    Sprite(const Sprite &old);
    ~Sprite();

    void init() override;

    void draw() override;

    void copy(Entity &dst, const Entity &src) override {
        dst.addComponent<Sprite>(src.getComponent<Sprite>());
    }

    const bool equals(const Entity &e) override {
        return (*this == e.getComponent<Sprite>());
    }

    friend const bool operator==(const Sprite &s1, const Sprite &s2) noexcept {
        return s1._asset_name == s2._asset_name;
    }
    friend const bool operator!=(const Sprite &s1, const Sprite &s2) noexcept { return !(s1 == s2); }
private:
    Transform *_transform = nullptr;

    std::string _asset_name;
};

#endif /* _SPRITE_H_ */