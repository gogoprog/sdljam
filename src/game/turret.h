#pragma once

#include "../context.h"
#include "bullet.h"
#include "factory.h"

struct Turret : public Component {
    inline static String name = "Turret";
    float timeSinceLastFire{100};
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

        if (turret.timeSinceLastFire > 0.25 && Context::get().inputs.isMousePressed(1)) {
            turret.timeSinceLastFire = 0;
            auto e = Factory::createBullet();
            auto speed = 1000;
            auto angle = (rotatable.angle - 90) * std::numbers::pi / 180.0f;
            e->get<Bullet>().velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);
            e->position = entity.position;
            Context::get().engine.addEntity(e);
        }

        turret.timeSinceLastFire += dt;

        {
            auto delta = cursor_position - entity.position;
            rotatable.angle = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;

            if (turret.timeSinceLastFire < 0.2) {
                rotatable.frameOffset = 8;
            } else {
                rotatable.frameOffset = 0;
            }
        }
    }
};
