//  mingw32-make
//  ./app.exe

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
#include "ShadowMap.h"
#include <iostream>
#include <vector>
#include <memory>
#include "Scene.h"
#include "InputController.h"

SDL_Window* window;
SDL_GLContext glContext;

const int SCREEN_WIDTH  = 1080;
const int SCREEN_HEIGHT = 800;

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
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL
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

const float CAMERA_SPEED     = 1.0f;


void PreDraw()
{
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Main_Loop()
{   
    Scene scene;
    scene.BuildScene("scenes/main_scene.json");

    InputController inputController;
    SDL_SetRelativeMouseMode(SDL_TRUE);

    std::shared_ptr<Shader> depthShader = std::make_shared<Shader>("./shaders/Depth_Vert.glsl", "./shaders/Depth_Frag.glsl");
    ShadowMap shadowMap;

    Camera camera(
        glm::vec3(0.0f, 0.0f, 10.0f), //position
        glm::vec3(0.0f, 0.0f, -1.0f), //forward
        glm::vec3(0.0f, 1.0f, 0.0f), //up
        75.0f, //fov
        1080.0f / 800.0f, //aspect ratio
        0.1f, //near plane
        100.0f //far plane
    );
    

    Uint32 lastTime = SDL_GetTicks();

    while(!inputController.WantsQuit() && !inputController.IsPressed(SDL_SCANCODE_ESCAPE))
    {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;  // ms -> seconds
        lastTime = currentTime;

        inputController.ReadInputs();

       // Camera look
        float mouseDX = inputController.GetMouseDeltaX();
        float mouseDY = inputController.GetMouseDeltaY();
        if(mouseDX != 0.0f || mouseDY != 0.0f)
            camera.Rotate(mouseDX, mouseDY);
       

        // Camera movement
        if(inputController.IsHeld(SDL_SCANCODE_W))    camera.MoveForward( CAMERA_SPEED* deltaTime);
        if(inputController.IsHeld(SDL_SCANCODE_S))    camera.MoveForward(-CAMERA_SPEED* deltaTime);
        if(inputController.IsHeld(SDL_SCANCODE_A))    camera.MoveRight(  -CAMERA_SPEED* deltaTime);
        if(inputController.IsHeld(SDL_SCANCODE_D))    camera.MoveRight(   CAMERA_SPEED* deltaTime);
        if(inputController.IsHeld(SDL_SCANCODE_Q))   camera.MoveUp(      CAMERA_SPEED* deltaTime);
        if(inputController.IsHeld(SDL_SCANCODE_E)) camera.MoveUp(     -CAMERA_SPEED* deltaTime);

        //monkey.transform.rotation.z += 0.01f;

        //Shadow map stuff
        glm::mat4 lightView = glm::lookAt(
        scene.lights.position,           // light's position
        glm::vec3(0.0f),          // looking at the origin (where your cube is)
        glm::vec3(0.0f, 1.0f, 0.0f) // up vector
        );

        glm::mat4 lightProjection = glm::perspective(
        glm::radians(90.0f),  // fov — wide enough to cover the scene
        1.0f,                 // aspect ratio — 1:1 since shadow map is square
        1.0f,                 // near plane
        100.0f                // far plane, match your camera's
        );  

        glm::mat4 lightSpaceMatrix = lightProjection * lightView;

        shadowMap.Bind();

        scene.DrawDepth(depthShader, lightSpaceMatrix);
       
        
        shadowMap.Unbind(SCREEN_WIDTH, SCREEN_HEIGHT);
        
        PreDraw();

        scene.UpdateScene(deltaTime);

        scene.Draw(camera, shadowMap.GetTexture(), lightSpaceMatrix);
        
        

        SDL_GL_SwapWindow(window);
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