#pragma once

#include "../context.h"

#include "sprite.h"

struct Shake : public Component {
    inline static String name = "Shake";

    float duration = 1.0f;
    float intensity = 2.0f;

    float time{0};
    Vector2 initialPosition;
};

class ShakeSystem : public System {
  public:
    ShakeSystem() {
        require<Shake>();
    }

    void onEntityAdded(Entity &entity) override {
        auto &shake = entity.get<Shake>();
        shake.initialPosition = entity.position;
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &shake = entity.get<Shake>();

        shake.time += dt;

        /* auto intensity = shake.intensity / (shake.time / shake.duration); */
        auto intensity = shake.intensity;

        float x = noise(shake.time * 10, 2) * intensity;
        float y = noise(shake.time * 10, 3) * intensity;

        entity.position = shake.initialPosition + Vector2{x, y};

        if (shake.time > shake.duration) {
            entity.position = shake.initialPosition;
            entity.remove<Shake>();
        }
    }

  private:
    static float noise(const float v, const float f) {
        return std::sin(f * v) + std::sin(std::numbers::pi * v);
    }
};
