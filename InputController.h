#ifndef INPUT_CONTROLLER_H
#define INPUT_CONTROLLER_H

#include <SDL2/SDL.h>
#include <array>

#define MOUSE_SENSITIVITY 0.1f // adjust to taste, matches old constant

enum class KeyState
{
    Up,
    Pressed,
    Held,
    Released
};

class InputController
{
    public:
        InputController();

        void ReadInputs();

        bool IsPressed(SDL_Scancode key) const;
        bool IsHeld(SDL_Scancode key) const;
        bool IsReleased(SDL_Scancode key) const;
        KeyState GetKeyState(SDL_Scancode key) const;

        float GetMouseDeltaX() const;
        float GetMouseDeltaY() const;

        bool WantsQuit() const;

    private:
        std::array<KeyState, SDL_NUM_SCANCODES> keyStates;
        bool quitRequested;

        float mouseDeltaX;
        float mouseDeltaY;
};

#endif