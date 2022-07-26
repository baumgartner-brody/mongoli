#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include <stdint.h>

/* Definitions for the animation struct */
/* _index: An index to refer to an animation */
/* _num_frames: The number of horizontal frames this animation has */
/* _speed: The delay in MS for each frame */
/* _y: The upper left corner where a horizontal series of frames begins. */

struct Animation {
public:

    Animation() = default;
    Animation(const uint8_t &index, const uint8_t &num_frames, const unsigned int &speed, const unsigned int &y) :
        _index(index), _num_frames(num_frames), _speed(speed), _y(y) {}
    ~Animation() = default;

    uint8_t _index = uint8_t(0u);
    uint8_t _num_frames = uint8_t(1u);
    unsigned int _speed = 500u;
    unsigned int _y = 0u;
};

#endif /* _ANIMATION_H_ */