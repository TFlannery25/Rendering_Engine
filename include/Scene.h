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
#include "Component.h"
#include "Components/all_components.h"
#include <unordered_map>
#include "json.hpp"
#include <fstream>
#include "UpdateContext.h"

using json = nlohmann::json;
using ComponentFactory = std::function<std::unique_ptr<Component>(const json&)>;
    

class Scene
{
    private:
        std::unordered_map<std::string, ComponentFactory> componentFactories;

    public:
        std::vector<Object> objects;
        Light lights; // std::vector<Light> for multiple lights later

        Scene() {BuildComponentFactories();}

        void BuildComponentFactories();

        void BuildScene(const std::string& sceneFile);
        void UpdateScene(const UpdateContext& updateContext);

        void Draw(const Camera& camera, GLuint shadowMap, const glm::mat4& lightSpaceMatrix);
        void DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix);
};

#endif