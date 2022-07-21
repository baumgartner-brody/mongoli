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
    Sprite(const std::string &asset_name, const int &srcR_x, const int &srcR_y, const int &srcR_w, const int &srcR_h);
    Sprite(const std::string &asset_name, const SDL_Rect &srcR);
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
        if (s1._texture != s2._texture) return false;

        //if (s1._srcR != s2._srcR) return false;

        return true;
    }
    friend const bool operator!=(const Sprite &s1, const Sprite &s2) noexcept { return !(s1 == s2); }
private:
    Transform *_transform = nullptr;

    SDL_Rect _srcR;

    SDL_Texture *_texture = nullptr;
};

#endif /* _SPRITE_H_ */