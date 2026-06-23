#include "Scene.h"


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

    lights = light;
}

void Scene::Draw(const Camera& camera, GLuint shadowMap, const glm::mat4& lightSpaceMatrix)
{
    for (auto obj: objects)
    {
        obj.Draw(camera, lights, shadowMap, lightSpaceMatrix);
    }
}


void Scene::DrawDepth(std::shared_ptr<Shader> depthShader, const glm::mat4& lightSpaceMatrix)
{
    for (auto obj: objects)
    {
        obj.DrawDepth(depthShader, lightSpaceMatrix);
    }
}