#ifndef OBJECT_H
#define OBJECT_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Transform.h"
#include "Mesh.h"
#include "Shader.h"
#include "Camera.h"
#include "Light.h"
#include "ShadowLight.h"
#include <memory>
#include "Component.h"
#include "UpdateContext.h"

class Object
{
    public:
    Transform transform;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;
    std::vector<std::unique_ptr<Component>> components;

    Object(Transform &t, std::shared_ptr<Mesh> m, std::shared_ptr<Shader> s) : transform(t), mesh(m), shader(s) {}

    void Update(const UpdateContext& updateContext);
    void Draw(const Camera& camera, const std::vector<Light>& illuminationLights, const std::vector<ShadowLight>& shadowLights);
    void DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix);

};

#endif