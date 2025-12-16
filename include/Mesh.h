#ifndef MESH_H 
#define MESH_H 

#include "glad/glad.h" 
#include <vector>

class Mesh{ 
    private: 
    GLuint VAO; 
    GLuint VBO; 
    int numVertices; 
    
    public: 
    Mesh(std::vector<GLfloat> vertices); 
    ~Mesh(); 
    void Draw() const; 
}; 

#endif