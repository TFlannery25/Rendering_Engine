#ifndef VERTEX_H
#define VERTEX_H

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec3 specColor;
    float shininess;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color, glm::vec3 specColor, float shininess){
        this->position = position;
        this->normal = normal;
        this->color = color;
        this->specColor = specColor;
        this->shininess = shininess;
    }

};


#endif