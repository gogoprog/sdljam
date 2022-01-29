#pragma once

#include "../context.h"

struct Bullet : public Component {
    inline static String name = "Bullet";
    Vector2 velocity;
};

class BulletSystem : public System {
  public:
    BulletSystem() {
        componentsNames.push_back("Bullet");
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &bullet = entity.get<Bullet>();

        entity.position = entity.position + bullet.velocity * dt;

        Context::get().renderer.draw(entity.position, "Bullets", 0);
    }
};
