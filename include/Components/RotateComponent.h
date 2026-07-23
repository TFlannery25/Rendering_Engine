#ifndef ROTATE_COMPONENT_H
#define ROTATE_COMPONENT_H

#include "../Component.h"
#include "../Object.h"
#include <glm/glm.hpp>
#include "../UpdateContext.h"   

class RotateComponent : public Component
{
    glm::vec3 speed;
    public:
        RotateComponent(glm::vec3 s) : speed(s) {}
        void Update(Object& owner, const UpdateContext& updateContext) override
        {
            owner.transform.rotation += speed * updateContext.deltaTime;
        }
};

#endif