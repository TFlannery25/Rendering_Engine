#ifndef CAMERA_H
#define CAMERA_H
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>

class Camera {

    private:
        glm::vec3 position;
        glm::vec3 forward;
        glm::vec3 up;
        glm::vec3 right;

        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        float yaw   = -90.0f; // degrees; -90 so default forward is -Z
        float pitch =   0.0f;

        void UpdateVectors(); // recalculates forward/right/up from yaw+pitch

    public:

        Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 upDir, float fieldOfView, float aspect, float nearP, float farP);

        // Movement
        void MoveForward(float amount);
        void MoveRight(float amount);
        void MoveUp(float amount);

        // Look (delta degrees)
        void Rotate(float deltaYaw, float deltaPitch);

        glm::mat4 GetView()       const { return glm::lookAt(position, position + forward, up); }
        glm::mat4 GetProjection() const { return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane); }
        glm::vec3 GetPosition()   const { return position; }
};


#endif