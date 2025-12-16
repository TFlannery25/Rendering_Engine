#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "include/glad/glad.h"
#include "include/Shader.h"
#include "include/Mesh.h"
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
        800, 600, SDL_WINDOW_OPENGL
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

void Input(bool &quit)
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
    }
}
void PreDraw()
{
    //sets the clear color and clears the screen
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f); //sets clear color
    //glClear(GL_COLOR_BUFFER_BIT); //clears the color buffer
}

void Main_Loop()
{
    std::vector<GLfloat> vertices = {
    // positions (x, y, z)
     0.5f,  0.5f, 0.0f,
     0.5f, -0.5f, 0.5f,
     -0.5f, -0.5f, 0.0f, //};

     -0.5f,  0.5f, 0.0f,
     0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.0f };

    Mesh triangle(vertices);
    Shader shader("./shaders/Vertex.glsl", "./shaders/Fragment.glsl");
    bool quit = 0;

    while(!quit)
    {
        Input(quit);
        PreDraw();
        shader.Use();
        triangle.Draw();
        shader.Unuse();
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