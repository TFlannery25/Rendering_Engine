#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

struct Transform {
    glm::vec3 position{0,0,0};
    glm::vec3 rotation{0,0,0}; // Euler angles in radians
    glm::vec3 scale{1,1,1};

    glm::mat4 GetMatrix() const;
};

#endif