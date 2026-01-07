#ifndef VERTEX_H
#define VERTEX_H

struct Vertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;

    Vertex(glm::vec3 position, glm::vec3 normal, glm::vec3 color){
        this->position = position;
        this->normal = normal;
        this->color = color;
    }

};


#endif