#ifndef _TRANSFORM_COMPONENT_H_
#define _TRANSFORM_COMPONENT_H_

#include "ECS.h"

class TransformComponent : public Component {
public:

    /* Value constructor for TransformComponent */
    TransformComponent(const int &xpos, const int &ypos, const int &w, const int &h);

    /* Copy constructor */
    TransformComponent(const TransformComponent &old);

    /* Copy assignment operator */
    const TransformComponent& operator=(const TransformComponent &old);

    ~TransformComponent();

    void copy(Entity &dst, const Entity &old) override;

    int xpos, ypos;
    int width, height;

    /* Equality operators for Transform */
    friend const bool operator==(const TransformComponent &t1, const TransformComponent &t2) noexcept;
    friend const bool operator!=(const TransformComponent &t1, const TransformComponent &t2) noexcept;

    const bool equals(const Entity &e) override { return (*this == e.getComponent<TransformComponent>()); }
private:
    /* Does the backend work of a copy operation */
    void _copy(const TransformComponent &old);
};

#endif /* _TRANSFORM_COMPONENT_H_ */