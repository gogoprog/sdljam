#pragma once

#include "../context.h"
#include "move.h"
#include "factory.h"

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

        auto &spawn = entity.get<Spawn>();

        spawn.timeSinceLastSpawn += dt;

        if (spawn.timeSinceLastSpawn > 1.0) {

            auto e = Factory::createVehicle();
            e->position = entity.position;
            engine->addEntity(e);

            spawn.timeSinceLastSpawn = 0;
        }
    }
};
