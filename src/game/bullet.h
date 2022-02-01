#pragma once

#include "../context.h"

#include "hittable.h"
#include "life.h"

struct Bullet : public Component {
    inline static String name = "Bullet";
    Vector2 velocity;
    float lifetime{0};
};

class BulletSystem : public System {
  public:
    BulletSystem() {
        componentsNames.push_back("Bullet");
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &bullet = entity.get<Bullet>();

        entity.position = entity.position + bullet.velocity * dt;

        bullet.lifetime += dt;

        if (bullet.lifetime > 5.0) {
            engine->removeEntity(entity);
            return;
        }

        engine->iterate<Hittable>([&](Entity &other_entity) {
            auto delta = other_entity.position - entity.position;

            if (delta.getSquareLength() < 24 * 24) {
                auto e = Factory::createSmallExplosion();
                e->position = entity.position;
                engine->addEntity(e);

                other_entity.get<Life>().hp -= 1;
                engine->removeEntity(entity);

                return false;
            }

            return true;
        });
    }
};
