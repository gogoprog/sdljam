#pragma once

#include "../context.h"
#include "bullet.h"
#include "factory.h"
#include "sprite.h"

struct Turret : public Component {
    inline static String name = "Turret";
    float timeSinceLastFire{100};
    bool mustFire{false};
};

class TurretSystem : public System {
  public:
    TurretSystem() {
        componentsNames.push_back("Turret");
        componentsNames.push_back("RotatableSprite");
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();
        auto &rotatable = entity.get<RotatableSprite>();

        if (turret.timeSinceLastFire > 0.25 && turret.mustFire) {
            turret.timeSinceLastFire = 0;
            auto e = Factory::createBullet();
            auto speed = 500;
            auto angle = (rotatable.angle - 90) * std::numbers::pi / 180.0f;
            auto velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);
            auto direction = velocity.getNormalized();
            e->get<Bullet>().velocity = velocity;
            e->get<RotatableSprite>().angle = rotatable.angle;
            e->position = entity.position + direction * 32;
            Context::get().engine.addEntity(e);
            turret.mustFire = false;
        }

        turret.timeSinceLastFire += dt;

        if (turret.timeSinceLastFire < 0.2) {
            rotatable.frameOffset = 8;
        } else {
            rotatable.frameOffset = 0;
        }
    }
};
