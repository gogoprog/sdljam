#pragma once

#include "../context.h"
#include "bullet.h"
#include "factory.h"
#include "sprite.h"

struct Movable : public Component {
    inline static String name = "Movable";
};

struct Move : public Component {
    inline static String name = "Move";

    Move(Vector2 f, Vector2 t, float s) : from(f), to(t), speed(s) {
    }

    Vector2 from;
    Vector2 to;
    float speed;
    float time{0};
    float duration;
};

class MoveSystem : public System {
  public:
    MoveSystem() {
        componentsNames.push_back("Move");
        componentsNames.push_back("RotatableSprite");
    }

    void onEntityAdded(Entity &entity) override {
        auto &rotatable = entity.get<RotatableSprite>();
        auto &move = entity.get<Move>();

        auto delta = move.to - move.from;
        rotatable.angle = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;

        move.duration = delta.getLength() / move.speed;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &move = entity.get<Move>();

        if (move.time < move.duration) {
            move.time += dt;
            auto progress = move.time / move.duration;
            entity.position = move.from + (move.to - move.from) * progress;

            if (progress >= 1) {
                entity.remove<Move>();
            }
        }
    }
};
