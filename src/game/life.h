#pragma once

#include "../context.h"

#include "factory.h"
#include "shake.h"
#include <cstdlib>

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
            auto &cameraEntity = Context::get().cameraEntity;
            auto &game = Context::get().game;
            auto e = Factory::createExplosion();

            e->position = entity.position;
            engine->addEntity(e);

            engine->removeEntity(entity);

            game.stats.money += 20 + 20 * game.waveCount;

            Context::get().audio.playSound("impact");

            if (!cameraEntity->has<Shake>()) {
                cameraEntity->add<Shake>();

                // TODO: Maybe this should be in the constructor for Shake
                // instead with a static variable making it bigger every Nth
                // trigger.
                // I'd also add a freeze-frame effect as well, but that's
                // another topic.
                if (rand() % 100 >= 70) {
                    cameraEntity->get<Shake>().intensity += 0.8f;
                    cameraEntity->get<Shake>().duration += 1.0f;
                }
            }
        }
    }
};
