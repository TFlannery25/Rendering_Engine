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

class Object
{
    public:
    Transform transform;
    Mesh mesh;
    Shader shader;

    Object(Transform &t, Mesh &m, Shader &s) : transform(t), mesh(m), shader(s) {}

    void Draw(const Camera& camera, const Light& light);


};

#endif