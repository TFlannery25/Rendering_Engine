#ifndef ROTATE_COMPONENT_H
#define ROTATE_COMPONENT_H

#include "../Component.h"
#include "../Object.h"
#include <glm/glm.hpp>

class RotateComponent : public Component
{
    glm::vec3 speed;
    public:
        RotateComponent(glm::vec3 s) : speed(s) {}
        void Update(Object& owner, float deltaTime)
        {
            owner.transform.rotation += speed * deltaTime;
        }
};

#endif