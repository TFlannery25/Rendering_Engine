#ifndef LIGHT_H
#define LIGHT_H

class Light{
    
    public: 
    glm::vec3 position;
    glm::vec3 color;
    float intensity;
    //add a mesh for the light maybe

    Light(glm::vec3 pos, glm::vec3 col, float intense) :
    position(pos), color(col), intensity(intense) {}

};


#endif