#include "game.h"

#include "context.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/factory.h"
#include "game/sprite.h"
#include "game/turret.h"
#include "game/ui.h"

void Game::init() {
    auto &engine = Context::get().engine;

    engine.addSystem(new TurretSystem());
    engine.addSystem(new BulletSystem());
    engine.addSystem(new CameraSystem());
    engine.addSystem(new ControlSystem());
    engine.addSystem(new SpriteRotaterSystem());
    engine.addSystem(new SpriteRendererSystem());
    engine.addSystem(new UiSystem());

    {
        auto e = Factory::createCamera();
        engine.addEntity(e);
    }

    {
        auto e = Factory::createBase();
        e->position = {128, 256};
        engine.addEntity(e);
    }

    for (int i = 0; i < 10; ++i) {
        Vector2 pos = {rand() % 1024, rand() % 1024};

        {
            auto e = Factory::createBase();
            e->position = pos;
            engine.addEntity(e);
        }

        {
            auto e = Factory::createTurret();
            e->position = pos;
            engine.addEntity(e);
        }
    }
}
