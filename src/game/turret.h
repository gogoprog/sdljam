#pragma once

#include "../context.h"
#include "bullet.h"

struct Turret : public Component {
    inline static String name = "Turret";
    float angle{0};
    float timeSinceLastFire{100};
};

class TurretSystem : public System {
  public:
    TurretSystem() {
        componentsNames.push_back("Turret");
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();

        if (turret.timeSinceLastFire > 0.25 && Context::get().inputs.isMousePressed(1)) {
            turret.timeSinceLastFire = 0;
            auto e = std::make_shared<Entity>();
            e->add<Bullet>();
            auto speed = 1000;
            auto angle = (turret.angle - 90) * std::numbers::pi / 180.0f;
            e->get<Bullet>().velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);
            e->position = entity.position;
            Context::get().engine.addEntity(e);
        }

        turret.timeSinceLastFire += dt;

        {
            auto frame = 2;
            auto delta = cursor_position - entity.position;
            turret.angle = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;
            /* turret.angle += dt * 100; */
            turret.angle = clampAngle(turret.angle);

            auto step = 45 / 2.0f;
            auto angle = turret.angle;

            if (angle < step) {
                frame = 2;
            } else if (angle < step * 3) {
                frame = 3;
            } else if (angle < step * 5) {
                frame = 5;
            } else if (angle < step * 7) {
                frame = 6;
            } else if (angle < step * 9) {
                frame = 7;
            } else if (angle < step * 11) {
                frame = 8;
            } else if (angle < step * 13) {
                frame = 9;
            } else if (angle < step * 15) {
                frame = 4;
            }

            if (turret.timeSinceLastFire < 0.2) {
                frame += 8;
            }

            Context::get().renderer.draw(entity.position, "Turret", 0);
            Context::get().renderer.draw(entity.position, "Turret", frame);
        }
    }
};
