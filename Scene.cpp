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


    lights = Light(
    glm::vec3(data["lights"][0]["position"][0], data["lights"][0]["position"][1], data["lights"][0]["position"][2]),
    glm::vec3(data["lights"][0]["color"][0], data["lights"][0]["color"][1], data["lights"][0]["color"][2]),
    data["lights"][0]["intensity"]
    );

}

void Scene::UpdateScene(const UpdateContext& updateContext)
{
    for(auto& obj : objects)
        obj.Update(updateContext);
}

void Scene::Draw(const Camera& camera, GLuint shadowMap, const glm::mat4& lightSpaceMatrix)
{
    for (auto& obj: objects)
    {
        obj.Draw(camera, lights, shadowMap, lightSpaceMatrix);
    }
}

void Scene::DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix)
{
    for (auto& obj: objects)
    {
        obj.DrawDepth(depthShader, lightSpaceMatrix);
    }
}