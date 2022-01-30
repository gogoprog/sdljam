#pragma once

#include "../context.h"

struct Camera : public Component {
    inline static String name = "Camera";
};

class CameraSystem : public System {
  public:
    CameraSystem() {
        componentsNames.push_back("Camera");
    }

    void updateSingle(const float dt, Entity &entity) override {
        Vector2 position = {int(entity.position.x), int(entity.position.y)};
        auto &renderer = Context::get().renderer;
        auto &level = Context::get().level;

        if (renderer.width < level.width) {
            position.x = std::clamp<int>(position.x, 0, level.width - renderer.width);
        }

        if (renderer.height < level.height) {
            position.y = std::clamp<int>(position.y, 0, level.height - renderer.height);
        }

        entity.position = position;

        Context::get().renderer.getCameraPosition() = position;
    }
};
