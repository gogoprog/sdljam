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
        Context::get().renderer.getCameraPosition() = {int(entity.position.x), int(entity.position.y)};
    }
};
