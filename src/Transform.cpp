#include "Transform.h"


glm::mat4 Transform::GetMatrix() const {
        glm::mat4 mat(1.0f);
        mat = glm::translate(mat, position);
        mat = glm::rotate(mat, rotation.x, {1,0,0});
        mat = glm::rotate(mat, rotation.y, {0,1,0});
        mat = glm::rotate(mat, rotation.z, {0,0,1});
        mat = glm::scale(mat, scale);
        return mat;
    }