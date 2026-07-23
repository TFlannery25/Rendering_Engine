#ifndef UPDATECONTEXT_H
#define UPDATECONTEXT_H

#include "InputController.h"


// Holds the context for updating objects, including delta time and input controller 
// If more context is needed for updating objects, such as additional timing information or other input states, it can be added here.
// Created and updated each frame in main
struct UpdateContext
{
    float deltaTime;
    const InputController& inputController;

};

#endif