#pragma once

#include "../context.h"
#include "move.h"

struct Vehicle : public Component {
    inline static String name = "Vehicle";
};

class VehicleSystem : public System {
  public:
    VehicleSystem() {
        componentsNames.push_back("Vehicle");
    }

    void onEntityAdded(Entity &entity) override {
        /* auto &move = entity.add<Move>(entity.position, Vector2{350, 256}, 100); */
    }

    void updateSingle(const float dt, Entity &entity) override {

        if (!entity.has<Move>()) {
            entity.add<Move>(entity.position, Vector2{rand() % 1024, rand() % 1024}, 100);
        }
    }
};
