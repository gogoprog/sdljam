#pragma once

#include "context.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/turret.h"

class Game {
  public:
    static void init() {
        auto &engine = Context::get().engine;
        engine.addSystem(new TurretSystem());
        engine.addSystem(new CameraSystem());
        engine.addSystem(new ControlSystem());

        {
            auto e = std::make_shared<Entity>();
            e->add<Camera>();
            e->add<Control>();
            e->position = {0, 0};
            engine.addEntity(e);
        }

        {
            auto e = std::make_shared<Entity>();
            e->add<Turret>();
            e->position = {128, 256};
            engine.addEntity(e);
        }
        {
            auto e = std::make_shared<Entity>();
            e->add<Turret>();
            e->position = {512, 256};
            engine.addEntity(e);
        }
    }
};
