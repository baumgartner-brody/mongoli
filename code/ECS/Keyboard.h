#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_

#include "ECS.h"
#include "Transform.h"

class KeyboardController : public Component {
public:
    KeyboardController() = default;
    ~KeyboardController();

    void init() override;

    void update() override;

    /* No draw function needed */

private:
    TransformComponent *_transform = nullptr;

    /* This will point to the same place as Game::event */
    SDL_Event *_event = nullptr;

    /* Create a shallow copy of old into *this */
    void _copy(const KeyboardController &old) noexcept;
};

#endif /* _KEYBOARD_H_ */