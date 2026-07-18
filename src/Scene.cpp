#include "Scene.h"

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
                if (type == "RotateComponent")
                {
                    glm::vec3 speed(componentData["speed"][0], componentData["speed"][1], componentData["speed"][2]);
                    newObject.components.push_back(std::make_unique<RotateComponent>(speed));
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
 /*
void Scene::BuildScene()
{
    
   
    std::vector<Vertex> CubeVertices;
    std::vector<GLuint> CubeIndices;
    std::vector<Vertex> MonkeyVertices;
    std::vector<GLuint> MonkeyIndices;

    LoadOBJ("./Cube.obj", CubeVertices, CubeIndices);
    LoadOBJ("./ShinyMonkey.obj", MonkeyVertices, MonkeyIndices);

    std::shared_ptr<Mesh> cubeMesh = std::make_shared<Mesh>(CubeVertices, CubeIndices);
    Transform CubeTransform;
    CubeTransform.position = {0.0, 0.0, 0.0};
    CubeTransform.scale = {5.0f, 5.0f, 1.0f};

    std::shared_ptr<Mesh> monkeyMesh = std::make_shared<Mesh>(MonkeyVertices, MonkeyIndices);
    Transform monkeyTransform;
    monkeyTransform.position = {0.0, 0.0, 5.0};
    monkeyTransform.scale = {1.0f, 1.0f, 1.0f};

    Light light(
        glm::vec3(1.0f, 0.0f, 15.0f), //position
        glm::vec3(1.0f, 1.0f, 1.0f), //color
        1.0f //intensity
    );

    Transform lightTrans;
    lightTrans.position = light.position;
    lightTrans.scale = {0.2f, 0.2f, 0.2f};

    std::shared_ptr<Shader> shader = std::make_shared<Shader>("./shaders/Vertex.glsl", "./shaders/Fragment.glsl");

    objects.emplace_back(lightTrans, cubeMesh, shader);
    objects.emplace_back(CubeTransform, cubeMesh, shader);
    objects.emplace_back(monkeyTransform, monkeyMesh, shader);

    objects[2].components.push_back(std::make_shared<RotateComponent>(glm::vec3(0.0f, 1.0f, 0.0f)));

    lights = light;
    
}
*/
void Scene::UpdateScene(float deltaTime)
{
    for(auto& obj : objects)
        obj.Update(deltaTime);
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