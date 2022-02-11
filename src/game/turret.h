#pragma once

#include "../context.h"
#include "bullet.h"
#include "factory.h"
#include "sprite.h"

struct TurretStat {
    float interval;
    float range;
    float damage;
};

extern Array<TurretStat, 10> turretStats;

struct Turret : public Component {
    inline static String name = "Turret";
    float timeSinceLastFire{100};
    bool mustFire{false};
    int level{0};
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
        auto &renderer = Context::get().renderer;
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();
        auto &rotatable = entity.get<RotatableSprite>();
        auto &stat = turretStats[turret.level];

        if (turret.timeSinceLastFire > stat.interval && turret.mustFire) {
            turret.timeSinceLastFire = 0;
            auto e = Factory::createBullet();
            auto speed = 500;
            auto angle = (rotatable.angle - 90) * std::numbers::pi / 180.0f;
            auto velocity = Vector2(std::cos(angle) * speed, std::sin(angle) * speed);
            auto direction = velocity.getNormalized();
            e->get<Bullet>().velocity = velocity;
            e->get<Bullet>().lifetimeLeft = stat.range / speed;
            e->get<Bullet>().damage = stat.damage;
            e->get<RotatableSprite>().angle = rotatable.angle;
            e->position = entity.position + direction * 32;
            Context::get().engine.addEntity(e);
            turret.mustFire = false;

            Context::get().audio.playSound("firing");
        }

        turret.timeSinceLastFire += dt;

        if (turret.timeSinceLastFire < 0.2) {
            rotatable.frameOffset = 8;
        } else {
            rotatable.frameOffset = 0;
        }

        auto pos = entity.position;
        pos.y -= 32;
        pos.x -= 32;
        renderer.drawText(pos, std::to_string(turret.level).c_str(), 0.6, true, true);
    }
};
