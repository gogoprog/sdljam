#pragma once

#include "renderer.h"
#include "inputs.h"
#include "level.h"
#include "ecs.h"

class Context {
  public:
    Renderer renderer;
    Inputs inputs;
    Level level;
    Engine engine;

    static Context & get() {
        static Context instance;
        return instance;
    }
};
