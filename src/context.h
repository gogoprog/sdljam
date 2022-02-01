#pragma once

#include "renderer.h"
#include "inputs.h"
#include "level.h"
#include "ecs.h"
#include "game.h"

class Context {
  public:
    Renderer renderer;
    Inputs inputs;
    Level level;
    Engine engine;
    Game game;

    SharedPtr<Entity> cameraEntity;

    static Context & get() {
        static Context instance;
        return instance;
    }

    Vector2 getMouseWorldPosition() const {
      auto camera_position = renderer.getCameraPosition();
      auto mouse_position = inputs.getMousePosition();

      return mouse_position + camera_position;
    }
};
