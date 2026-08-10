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
#include "ShadowLight.h"

using json = nlohmann::json;
using ComponentFactory = std::function<std::unique_ptr<Component>(const json&)>;
    

class Scene
{
    private:
        std::unordered_map<std::string, ComponentFactory> componentFactories;
        const int MAX_SHADOW_LIGHTS = 4;

    public:
        std::vector<Object> objects;
        //Light lights; // std::vector<Light> for multiple lights later
        std::vector<Light> illuminationLights;
        std::vector<ShadowLight> shadowLights;

        Scene() {BuildComponentFactories();}

        void BuildComponentFactories();

        void BuildScene(const std::string& sceneFile);
        void UpdateScene(const UpdateContext& updateContext);

        void Draw(const Camera& camera);
        void DrawDepth(std::shared_ptr<Shader> depthShader, int SCREEN_WIDTH, int SCREEN_HEIGHT);
};

#endif