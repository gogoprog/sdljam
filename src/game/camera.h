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

        position.x = std::clamp<int>(position.x, 0, Context::get().level.width - Context::get().renderer.width);
        position.y = std::clamp<int>(position.y, 0, Context::get().level.height - Context::get().renderer.height);

        entity.position = position;

        Context::get().renderer.getCameraPosition() = position;
    }
};
