#include "Shader.h"

std::string Shader::LoadShader(const std::string& fileName)
{
    std::ifstream file(fileName);
    std::string shader;
    std::string line;

    if (!file.is_open())
    {
        std::cout << "Failed to open shader file: " << fileName << std::endl;
        return "";
    }

    while (std::getline(file, line))
    {
        shader += line + '\n';
    }

    return shader;
}

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
    GLint success;

    //vertex shader set up
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER); //create object
    std::string vertexShaderString = LoadShader(vertexPath);
    const char* vertexShaderSrc = vertexShaderString.c_str();
    glShaderSource(vertexShader, 1, &vertexShaderSrc, nullptr); //link source code
    glCompileShader(vertexShader); //compile

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, info);
        std::cerr << "Vertex Shader compile error:\n" << info << std::endl;
    }

    //fragment shader set up
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    std::string fragmentShaderString = LoadShader(fragmentPath);
    const char* fragmentShaderSrc = fragmentShaderString.c_str();
    glShaderSource(fragmentShader, 1, &fragmentShaderSrc, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char info[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, info);
        std::cerr << "Fragment Shader compile error:\n" << info << std::endl;
    }

    //create shader program
    GLuint program_ = glCreateProgram();
    glAttachShader(program_, vertexShader);
    glAttachShader(program_, fragmentShader);
    glLinkProgram(program_); //links them to the program

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    this->program = program_;
}

Shader::~Shader()
{
    glDeleteProgram(program);
}