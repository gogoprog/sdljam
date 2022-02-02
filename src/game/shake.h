#pragma once

#include "../context.h"

#include "sprite.h"

struct Shake : public Component {
    inline static String name = "Shake";

    float duration = 0.4f;
    float intensity = 0.3f;

    float time{0};
    Vector2 offset;
};

class ShakeSystem : public System {
  public:
    ShakeSystem() {
        require<Shake>();
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &shake = entity.get<Shake>();

        shake.time += dt;

        //TODO: This should scale with distance from screen borders as well.
        //Full force when on screen, linear decay when outside.
        auto intensity = shake.intensity / (shake.time / shake.duration);
        // auto intensity = shake.intensity;

        float x = noise(shake.time * 50, 2) * intensity;
        float y = noise(3, shake.time * 50) * intensity;

        entity.get<Shake>().offset =  Vector2{x, y};

        if (shake.time > shake.duration) {
            entity.remove<Shake>();
        }
    }

  private:
    static float noise(const float v, const float f) {
        return std::sin(f * v) + std::sin(std::numbers::pi * v);
    }
};
