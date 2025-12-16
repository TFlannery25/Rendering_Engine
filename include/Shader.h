#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <string>
#include <iostream>
#include <fstream>

class Shader{
    private:
        GLuint program;

        std::string LoadShader(const std::string& fileName);

    public:
        Shader(const std::string& vertexPath, const std::string& fragmentPath);
        ~Shader();

        void Use() { glUseProgram(program); }
        void Unuse() { glUseProgram(0); }
       
};

#endif