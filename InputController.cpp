#include "InputController.h"

InputController::InputController()
{
    keyStates.fill(KeyState::Up);
    quitRequested = false;
    mouseDeltaX = 0.0f;
    mouseDeltaY = 0.0f;
}

void InputController::ReadInputs()
{
    // Age states forward before applying this frame's events
    for (auto& state : keyStates)
    {
        if (state == KeyState::Pressed)
            state = KeyState::Held;
        else if (state == KeyState::Released)
            state = KeyState::Up;
    }

    mouseDeltaX = 0.0f;
    mouseDeltaY = 0.0f;

    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
        {
            quitRequested = true;
        }

        if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
        {
            keyStates[e.key.keysym.scancode] = KeyState::Pressed;
        }

        if (e.type == SDL_KEYUP)
        {
            keyStates[e.key.keysym.scancode] = KeyState::Released;
        }

        if (e.type == SDL_MOUSEMOTION)
        {
            mouseDeltaX += e.motion.xrel * MOUSE_SENSITIVITY;
            mouseDeltaY -= e.motion.yrel * MOUSE_SENSITIVITY; // invert Y: up = positive pitch
        }
    }
}

bool InputController::IsPressed(SDL_Scancode key) const
{
    return keyStates[key] == KeyState::Pressed;
}

bool InputController::IsHeld(SDL_Scancode key) const
{
    return keyStates[key] == KeyState::Pressed || keyStates[key] == KeyState::Held;
}

bool InputController::IsReleased(SDL_Scancode key) const
{
    return keyStates[key] == KeyState::Released;
}

KeyState InputController::GetKeyState(SDL_Scancode key) const
{
    return keyStates[key];
}

float InputController::GetMouseDeltaX() const
{
    return mouseDeltaX;
}

float InputController::GetMouseDeltaY() const
{
    return mouseDeltaY;
}

bool InputController::WantsQuit() const
{
    return quitRequested;
}