#ifndef SCENE_H
#define SCENE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "Object.h"
#include <memory>

class Scene
{
    public:
        std::vector<Object> objects;
        Light lights; // std::vector<Light> for multiple lights later

        Scene() {}

        void BuildScene();

        void Draw(const Camera& camera, GLuint shadowMap, const glm::mat4& lightSpaceMatrix);
        void DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix);
};

#endif