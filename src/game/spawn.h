#pragma once

#include "../context.h"
#include "factory.h"
#include "move.h"
#include "vehicle.h"

struct Spawn : public Component {
    inline static String name = "Spawn";

    float timeSinceLastSpawn{10000};
};

class SpawnSystem : public System {
  public:
    SpawnSystem() {
        componentsNames.push_back("Spawn");
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &game = Context::get().game;
        auto &wave = game.currentWave;

        auto &spawn = entity.get<Spawn>();

        spawn.timeSinceLastSpawn += dt;

        if (spawn.timeSinceLastSpawn > 1.0) {

            if (wave.units > 0) {
                auto e = Factory::createVehicle();
                e->position = entity.position;
                engine->addEntity(e);

                e->get<Vehicle>().speed = wave.speed;
                e->get<Life>().hp = wave.hp;

                wave.units--;
            } else {
                auto count = 0;

                engine->iterate<Vehicle>([&](auto &e) {
                    ++count;
                    return true;
                });

                if (!count) {
                    game.changeState(Game::State::WINNING);
                }
            }

            spawn.timeSinceLastSpawn = 0;
        }
    }
};
