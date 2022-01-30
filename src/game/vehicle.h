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
        auto &move = entity.add<Move>(entity.position, Vector2{1024, 1024}, 100);
    }

    void updateSingle(const float dt, Entity &entity) override {
    }
};
