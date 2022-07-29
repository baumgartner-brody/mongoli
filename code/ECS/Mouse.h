#ifndef _MOUSE_H_
#define _MOUSE_H_

#include "ECS.h"

/* Forward declaration of TransformComponent */
/* UNUSED */
class TransformComponent;

class HitboxComponent;

/* MouseComponent's parent entity is required to have a transform */
class MouseComponent : public Component {
public:
    MouseComponent() = default;
    MouseComponent(const unsigned int &width, const unsigned int &height);
    ~MouseComponent();

    void init() override;

    void update() override;

private:

    /* A pointer to the parent entity's transform component. */
    TransformComponent *_transform = nullptr;

    HitboxComponent *_hitbox = nullptr;

    unsigned int *_tmp_width_, *_tmp_height_;

    /* A pointer to Game::event */
    SDL_Event *_event = nullptr;

};

#endif /* _MOUSE_H_ */