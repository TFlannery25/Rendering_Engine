#ifndef MESH_H 
#define MESH_H 

#include "glad/glad.h" 
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Vertex.h"
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <map>

class Mesh{ 
    private: 
    GLuint VAO; 
    GLuint VBO; 
    GLuint EBO;
    int drawCount; 
    bool indexed = false;
    
    public: 
    Mesh(std::vector<Vertex> &vertices, std::vector<GLuint> &indices); 
    ~Mesh(); 
    void Draw() const; 
}; 

void LoadOBJ(const std::string& fileName, std::vector <Vertex>& vertices, std::vector<GLuint> &indices);

#endif