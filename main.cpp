#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "include/glad/glad.h"
#include "include/Shader.h"
#include "include/Mesh.h"
#include "Vertex.h"
#include "Transform.h"
#include "Object.h"
#include "Camera.h"
#include "Light.h"
#include <iostream>
#include <vector>

SDL_Window* window;
SDL_GLContext glContext;

void Initialize_Program()
{
    // initialize SDL video subsystem
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return;
    }

    // sets opengl context attributes
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE); // core profile, sets the OpenGL version
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // enable double buffering
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24); // 24-bit depth buffer 

    //creates the window
    window = SDL_CreateWindow(
        "SDL2 Test",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        1080, 800, SDL_WINDOW_OPENGL
    );

    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return;
    }

    //creates the opengl context
    glContext = SDL_GL_CreateContext(window);
    if (!glContext) {
    printf("Failed to create GL context: %s\n", SDL_GetError());
        return;
    }


    //initialize glad
    if(!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) //loads function pointers
    {
        printf("Failed to initialize glad: %s\n", SDL_GetError());
        return;
    }
    
}

void Input(bool &quit, bool &w, bool &s, bool &a, bool &d)
{
    //handles the input of closing the window
    SDL_Event e; 
    while(SDL_PollEvent(&e)) 
    {
        if(e.type == SDL_QUIT) // If user closes the window
        {
            std::cout << "Quit event received.\n";
            quit = true;
        }

        if(e.type == SDL_KEYDOWN) 
        {
            if(e.key.keysym.sym == SDLK_w)
            {
                w = true;
            }
            if(e.key.keysym.sym == SDLK_s)
            {
                s = true;
            }
            if(e.key.keysym.sym == SDLK_a)
            {
                a = true;
            }
            if(e.key.keysym.sym == SDLK_d)
            {
                d = true;
            }
        }
        if(e.type == SDL_KEYUP) 
        {
            if(e.key.keysym.sym == SDLK_w)
            {
                w = false;
            }
            if(e.key.keysym.sym == SDLK_s)
            {
                s = false;
            }
            if(e.key.keysym.sym == SDLK_a)
            {
                a = false;
            }
            if(e.key.keysym.sym == SDLK_d)
            {
                d = false;
            }
        }
    }
}
void PreDraw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Main_Loop()
{   
    std::vector<Vertex> vertices;
    std::vector<GLuint> indices;

    LoadOBJ("./Cube.obj", vertices, indices);
    
    Mesh Cube(vertices, indices);
    Transform transform;
    transform.position = {0.0, 0.0, 5.0};
    
    Shader shader("./shaders/Vertex.glsl", "./shaders/Fragment.glsl");
    Camera camera(
        glm::vec3(1.0f, 0.0f, 10.0f), //position
        glm::vec3(-0.1f, 0.0f, -1.0f), //forward
        glm::vec3(0.0f, 1.0f, 0.0f), //up
        75.0f, //fov
        1080.0f / 800.0f, //aspect ratio
        0.1f, //near plane
        100.0f //far plane
    );
    Light light(
        glm::vec3(1.0f, 0.0f, 9.0f), //position
        glm::vec3(1.0f, 1.0f, 1.0f), //color
        2.0f //intensity
    );
    Transform lightTrans;
    lightTrans.position = light.position;
    lightTrans.scale = {0.2f, 0.2f, 0.2f};
    Object lightObj(lightTrans, Cube, shader);

    Object cube(transform, Cube, shader);

    //Mesh Cube2(vertices, indices);
    //Transform transform2;
    //transform2.position = {1.0, 5.0, -5.0};
    //Object cube2(transform2, Cube2, shader);
    bool quit = 0;
    bool w = 0;
    bool s = 0;
    bool a = 0;
    bool d = 0;

    while(!quit)
    {
        Input(quit, w, s, a, d);
        PreDraw();
        
        cube.Draw(camera, light);
        //lightObj.Draw(camera, light);
        
        if(w)
        {
            cube.transform.rotation.x += 0.01f;
        }
        if(s)
        {
            cube.transform.rotation.x -= 0.01f;
        }
        if(a)
        {
            cube.transform.rotation.y -= 0.01f;
        }
        if(d)
        {
            cube.transform.rotation.y += 0.01f;
        }
        
        
        //GLuint modelLocation = glGetUniformLocation(shader.GetProgram(), "model");
        //glm::mat4 model = transform.GetMatrix();
        //glUniformMatrix4fv(modelLocation, 1, GL_FALSE, glm::value_ptr(model));

        //Cube.Draw();
        //shader.Unuse();
        
        
        SDL_GL_SwapWindow(window); //Swaps back buffer → front buffer (shows the frame).
    }
}

void Clean_Up()
{
    //glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteProgram(shaderProgram);
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int main(int argc, char* argv[]) {
    
    Initialize_Program();
    if(window == nullptr) {return 1;} //make sure window was created properly

    //VertexSpecification(VAO, VBO);

    Main_Loop();

    Clean_Up();

    return 0;
}