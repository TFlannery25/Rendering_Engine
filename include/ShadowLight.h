#ifndef SHADOW_LIGHT_H
#define SHADOW_LIGHT_H

#include "Light.h"
#include "ShadowMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class ShadowLight : public Light
{
    public:
        ShadowMap shadowMap;
        glm::mat4 lightSpaceMatrix;

        ShadowLight() : Light() { UpdateLightSpaceMatrix(); } //Default constuctor for uninitialized light (empty scene) dont render
        ShadowLight(glm::vec3 pos, glm::vec3 col, float intense) : Light(pos, col, intense) { UpdateLightSpaceMatrix(); }

        void UpdateLightSpaceMatrix()
        {
            glm::mat4 lightView = glm::lookAt(
                position,
                glm::vec3(0.0f),
                glm::vec3(0.0f, 1.0f, 0.0f)
            );

            glm::mat4 lightProjection = glm::perspective(
                glm::radians(90.0f),
                1.0f,
                1.0f,
                100.0f
            );

            lightSpaceMatrix = lightProjection * lightView;
        }
};

#endif