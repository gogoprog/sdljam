#include "game.h"

#include "context.h"
#include "game/animation.h"
#include "game/camera.h"
#include "game/control.h"
#include "game/factory.h"
#include "game/shake.h"
#include "game/spawn.h"
#include "game/sprite.h"
#include "game/state.h"
#include "game/turret.h"
#include "game/ui.h"
#include "game/vehicle.h"

struct Game::Pimpl {
    FiringStateSystem firingStateSystem;
    RoadBuildingStateSystem roadBuildingStateSystem;
    BuildingTurretsStateSystem buildingTurretsStateSystem;
    SpawnSystem spawnSystem;
    WinningStateSystem winningStateSystem;
    LosingStateSystem losingStateSystem;
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

    {
        auto e = Factory::createCamera();
        engine.addEntity(e);
        Context::get().cameraEntity = e;
    }
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
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.beginCoords);
        e->position.x += 98;
        e->position.y -= 40;
        engine.addEntity(e);
    }
    {
        auto e = Factory::createFlag();
        e->position = level.getTileCenterPosition(level.beginCoords);
        e->position.x -= 108;
        e->position.y -= 40;
        engine.addEntity(e);
    }

    if (0)
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

    stats.lifes = 25;
    stats.money = 1000;
    nextWave();
    changeState(State::INITIATING);
}

void Game::reset() {
    auto &level = Context::get().level;
    auto &engine = Context::get().engine;
    level = Level();

    engine.removeAllEntities();
    engine.removeAllSystems();

    waveCount = 0;

    init();
}

void Game::changeState(const State state) {
    auto &engine = Context::get().engine;

    switch (state) {
        case State::INITIATING: {
            changeState(State::BUILDING_ROADS);
        } break;

        case State::BUILDING_ROADS: {
            engine.removeSystem(&pimpl->winningStateSystem);
            engine.removeSystem(&pimpl->buildingTurretsStateSystem);
            engine.addSystem(&pimpl->roadBuildingStateSystem);
        } break;

        case State::BUILDING_TURRETS: {
            engine.removeSystem(&pimpl->roadBuildingStateSystem);
            engine.addSystem(&pimpl->buildingTurretsStateSystem);
        } break;

        case State::PLAYING: {
            engine.removeSystem(&pimpl->roadBuildingStateSystem);
            engine.removeSystem(&pimpl->buildingTurretsStateSystem);
            engine.addSystem(&pimpl->spawnSystem);
            engine.addSystem(&pimpl->firingStateSystem);
        } break;

        case State::WINNING: {
            engine.removeSystem(&pimpl->spawnSystem);
            engine.removeSystem(&pimpl->firingStateSystem);
            engine.addSystem(&pimpl->winningStateSystem);
        } break;

        case State::LOSING: {
            engine.removeSystem(&pimpl->spawnSystem);
            engine.removeSystem(&pimpl->firingStateSystem);
            engine.addSystem(&pimpl->losingStateSystem);
        } break;
    }
}

void Game::nextWave() {
    currentWave.units = (waveCount + 1) * 2;

    waveCount++;
}
