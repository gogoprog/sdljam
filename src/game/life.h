#pragma once

#include "../context.h"

#include "factory.h"

struct Life : public Component {
    inline static String name = "Life";

    int hp{10};
};

class LifeSystem : public System {
  public:
    LifeSystem() {
        componentsNames.push_back("Life");
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &life = entity.get<Life>();

        if (life.hp <= 0) {
            auto e = Factory::createExplosion();
            e->position = entity.position;
            engine->addEntity(e);

            engine->removeEntity(entity);
        }
    }
};
