#pragma once

#include "../context.h"

#include "hittable.h"
#include "life.h"

struct Bullet : public Component {
    inline static String name = "Bullet";
    Vector2 velocity;
    float lifetimeLeft{0};
    int damage;
};

class BulletSystem : public System {
  public:
    BulletSystem() {
        componentsNames.push_back("Bullet");
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &bullet = entity.get<Bullet>();

        entity.position = entity.position + bullet.velocity * dt;

        bullet.lifetimeLeft -= dt;

        if (bullet.lifetimeLeft < 0.0) {
            engine->removeEntity(entity);
            return;
        }

        engine->iterate<Hittable>([&](Entity &other_entity) {
            auto delta = other_entity.position - entity.position;

            if (delta.getSquareLength() < 24 * 24) {
                auto e = Factory::createSmallExplosion();
                e->position = entity.position;
                engine->addEntity(e);

                other_entity.get<Life>().hp -= bullet.damage;
                engine->removeEntity(entity);

                Context::get().audio.playSound("impact");

                return false;
            }

            return true;
        });
    }
};
