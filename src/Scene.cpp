#include "Scene.h"

// Builds the component factory by populating the componentFactories map with lambdas that create components from JSON data.
// When making components, add a corresponding entry in the componentFactories map so that the scene can create them from JSON data.
void Scene::BuildComponentFactories()
{
    componentFactories["RotateComponent"] = [](const json& componentData) -> std::unique_ptr<Component>
    {
    glm::vec3 speed(componentData["speed"][0], componentData["speed"][1], componentData["speed"][2]);
    return std::make_unique<RotateComponent>(speed);
    };
}

void Scene::BuildScene(const std::string& sceneFile)
{
    std::ifstream f(sceneFile);
    json data = json::parse(f);

    std::unordered_map<std::string, std::shared_ptr<Mesh>> meshCache;

    std::shared_ptr<Shader> shader = std::make_shared<Shader>(
        data["shader"]["vertex"], data["shader"]["fragment"]);

    for (auto& objData : data["objects"])
    {
        std::string meshPath = objData["mesh"];
        if (meshCache.find(meshPath) == meshCache.end())
        {
            std::vector<Vertex> verts;
            std::vector<GLuint> indices;
            LoadOBJ(meshPath, verts, indices);
            meshCache[meshPath] = std::make_shared<Mesh>(verts, indices);
        }

        Transform t;
        t.position = glm::vec3(objData["position"][0], objData["position"][1], objData["position"][2]);
        t.rotation = glm::radians(glm::vec3(objData["rotation"][0], objData["rotation"][1], objData["rotation"][2]));
        t.scale    = glm::vec3(objData["scale"][0], objData["scale"][1], objData["scale"][2]);

        Object newObject(t, meshCache[meshPath], shader);

        if (objData.contains("components"))
        {
            for (auto& componentData : objData["components"])
            {
                std::string type = componentData["type"];
                if (componentFactories.contains(type))
                {
                    newObject.components.push_back(componentFactories[type](componentData));
                }
            }
        }

        objects.push_back(std::move(newObject));
    }


    for (auto& objData : data["shadowLights"])
    {
        if ((int)shadowLights.size() >= MAX_SHADOW_LIGHTS)
        {
            std::cout << "Warning: exceeded max shadow lights (" << MAX_SHADOW_LIGHTS << "), ignoring extra entries." << std::endl;
            break;
        }
        shadowLights.emplace_back(
            glm::vec3(objData["position"][0], objData["position"][1], objData["position"][2]),
            glm::vec3(objData["color"][0], objData["color"][1], objData["color"][2]),
            objData["intensity"]
        );
    }

    for (auto& objData : data["illuminationLights"])
    {
        Light light;
        light.position = glm::vec3(objData["position"][0], objData["position"][1], objData["position"][2]);
        light.color = glm::vec3(objData["color"][0], objData["color"][1], objData["color"][2]);
        light.intensity = objData["intensity"];
        illuminationLights.push_back(light);
    }

   
}

void Scene::UpdateScene(const UpdateContext& updateContext)
{
    for(auto& obj : objects)
        obj.Update(updateContext);
}

void Scene::Draw(const Camera& camera)
{
    for (auto& obj: objects)
    {
        obj.Draw(camera, illuminationLights, shadowLights);
    }
}

void Scene::DrawDepth(std::shared_ptr<Shader> depthShader, int SCREEN_WIDTH, int SCREEN_HEIGHT)
{

    for (auto& shadowLight : shadowLights)
    {
        shadowLight.shadowMap.Bind();

        for (auto& obj: objects)
        {
            obj.DrawDepth(depthShader, shadowLight.lightSpaceMatrix);
        }

        shadowLight.shadowMap.Unbind(SCREEN_WIDTH, SCREEN_HEIGHT);
    }
}