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

        float fov;
        float aspectRatio;
        float nearPlane;
        float farPlane;

        bool printView = true;
        bool printProj = true;

    public:

        Camera(glm::vec3 pos, glm::vec3 fwd, glm::vec3 upDir, float fieldOfView, float aspect, float nearP, float farP) 
            : position(pos), forward(fwd), up(upDir), 
            fov(fieldOfView), aspectRatio(aspect), nearPlane(nearP), farPlane(farP) {}

        glm::mat4 GetView() const 
        { 
            return glm::lookAt(position, position + forward, up); 
            
        }
        glm::mat4 GetProjection() const 
        { 
            return glm::perspective(glm::radians(fov), aspectRatio, nearPlane, farPlane); 
        }

};


#endif