#pragma once

#include "types.h"
#include <SDL.h>
#include <map>
#include <memory>

class Inputs {
  public:
    Inputs() = default;
    ~Inputs() = default;

    void init();
    void update() {
        previousKeyStates = currentKeyStates;
        previousMouseButtonStates = currentMouseButtonStates;
    };

    void handle(const SDL_Event &event) {
        switch (event.type) {
            case SDL_KEYDOWN:
            case SDL_KEYUP: {
                auto &kevent = event.key;
                currentKeyStates[kevent.keysym.scancode] = kevent.state;
            } break;
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP: {
                auto &mevent = event.button;
                currentMouseButtonStates[mevent.button] = mevent.state;
            } break;
            case SDL_MOUSEMOTION: {
                auto &mevent = event.motion;
                mousePosition.x = mevent.x;
                mousePosition.y = mevent.y;

            } break;
        }
    }

    bool isKeyJustPressed(const int key) /*const*/ {
        return !previousKeyStates[key] && currentKeyStates[key];
    }

    bool isKeyJustReleased(const int key) /*const*/ {
        return previousKeyStates[key] && !currentKeyStates[key];
    }

    bool isKeyPressed(const int key) /*const*/ {
        return currentKeyStates[key];
    }

    bool isMouseJustPressed(const int button) /*const*/ {
        return !previousMouseButtonStates[button] && currentMouseButtonStates[button];
    }

    bool isMousePressed(const int button) /*const*/ {
        return currentMouseButtonStates[button];
    }

    const Vector2 &getMousePosition() const {
        return mousePosition;
    }

  private:
    std::map<uint32_t, bool> previousKeyStates;
    std::map<uint32_t, bool> currentKeyStates;
    std::map<uint8_t, bool> previousMouseButtonStates;
    std::map<uint8_t, bool> currentMouseButtonStates;

    Vector2 mousePosition;
};
