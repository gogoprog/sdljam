#pragma once

#include "context.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/turret.h"
#include "game/ui.h"

class Game {
  public:
    static void init() {
        auto &engine = Context::get().engine;
        engine.addSystem(new TurretSystem());
        engine.addSystem(new BulletSystem());
        engine.addSystem(new CameraSystem());
        engine.addSystem(new ControlSystem());
        engine.addSystem(new UiSystem());

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

        for (int i = 0; i < 10; ++i) {
            auto e = std::make_shared<Entity>();
            e->add<Turret>();
            e->position = {rand() % 1024, rand() % 1024};
            engine.addEntity(e);
        }
    }
};
