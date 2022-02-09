#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/factory.h"
#include "game/mode.h"
#include "game/shake.h"
#include "game/spawn.h"
#include "game/sprite.h"
#include "game/turret.h"
#include "game/ui.h"
#include "game/vehicle.h"

struct Game::Pimpl {
    FiringModeSystem firingModeSystem;
    RoadBuildingModeSystem roadBuildingModeSystem;
    SpawnSystem spawnSystem;
};

Game::Game() : pimpl(new Game::Pimpl()) {
}

Game::~Game() = default;

void Game::init() {
    auto &engine = Context::get().engine;
    auto &level = Context::get().level;

    engine.addSystem(new TurretSystem());
    engine.addSystem(new VehicleSystem());
    engine.addSystem(new BulletSystem());
    engine.addSystem(new LifeSystem());
    engine.addSystem(new MoveSystem());
    engine.addSystem(new CameraSystem());
    engine.addSystem(new ControlSystem());
    engine.addSystem(new AnimationSystem());
    engine.addSystem(new ShakeSystem());
    engine.addSystem(new SpriteRotaterSystem());
    engine.addSystem(new SpriteRendererSystem());
    engine.addSystem(new UiSystem());
    /* engine.addSystem(new ModeControlSystem()); */

    {
        auto e = Factory::createCamera();
        engine.addEntity(e);
        Context::get().cameraEntity = e;
    }

    /* for (int i = 0; i < 10; ++i) { */
    /*     auto e = Factory::createVehicle(); */
    /*     e->position = {128, 256}; */
    /*     engine.addEntity(e); */
    /* } */

    {
        auto e = Factory::createSpawn();
        e->position = level.getTileCenterPosition(level.beginCoords);
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.endCoords);
        e->position.x -= 108;
        e->position.y += 40;
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.endCoords);
        e->position.x += 98;
        e->position.y += 40;
        engine.addEntity(e);
    }

    for (int i = 0; i < 10; ++i) {
        Vector2 pos = {rand() % 2048, rand() % 2048};

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

    /* changeMode(Mode::FIRING); */
    changeState(State::PREPARING);
}

void Game::changeMode(const Mode mode) {
    auto &engine = Context::get().engine;

    switch (mode) {
        case Mode::FIRING:
            engine.removeSystem(&pimpl->roadBuildingModeSystem);
            engine.addSystem(&pimpl->firingModeSystem);
            break;
        case Mode::TURRET_BUILDING:
            break;
        case Mode::ROAD_BUILDING:
            engine.removeSystem(&pimpl->firingModeSystem);
            engine.addSystem(&pimpl->roadBuildingModeSystem);
            break;
    }
}

void Game::changeState(const State state) {
    auto &engine = Context::get().engine;

    switch (state) {
        case State::INITIATING: {

            changeState(State::PREPARING);
        } break;

        case State::PREPARING: {
            engine.addSystem(&pimpl->roadBuildingModeSystem);
        } break;

        case State::BUILDING: {

        } break;

        case State::PLAYING: {
            engine.removeSystem(&pimpl->roadBuildingModeSystem);
            engine.addSystem(&pimpl->spawnSystem);
        } break;

        case State::WINNING: {
        } break;

        case State::LOSING: {
        } break;
    }
}
