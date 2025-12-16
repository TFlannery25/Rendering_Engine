#include "Mesh.h" 

Mesh::Mesh(std::vector<GLfloat> vertices) 
{ 
        numVertices = static_cast<int>(vertices.size() / 3);

        //generates VAO and VBO objects 
        glGenVertexArrays(1, &VAO); 
        glGenBuffers(1, &VBO); 

        //bind (select) object 
        glBindVertexArray(VAO); 
        glBindBuffer(GL_ARRAY_BUFFER /*vertex attributes*/, VBO); 
        
        //upload vertex data to GPU 
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW); 

        //Attribute: 0 
        //3 floats per vertex 
        //No normalization 
        //Stride = 3 floats 
        //Offset = 0 
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0); 
        glEnableVertexAttribArray(0); //enables attribute location 0 
        
        //unbinds 
        glBindBuffer(GL_ARRAY_BUFFER, 0); 
        glBindVertexArray(0); 
        glDisableVertexAttribArray(0); 
    } 
    
    Mesh::~Mesh() 
    { 
        if (VBO != 0) 
        { 
            glDeleteBuffers(1, &VBO); 
        } 
        if (VAO != 0) 
        { 
            glDeleteVertexArrays(1, &VAO); 
        } 
    } 
    void Mesh::Draw() const 
    { 
        glBindVertexArray(VAO); 
        glDrawArrays(GL_TRIANGLES, 0, numVertices); //draw 6 vertices (2 triangles) 
        glBindVertexArray(0); 
    }