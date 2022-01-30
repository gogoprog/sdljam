#pragma once

#include "../context.h"
#include "sprite.h"
#include "turret.h"

class ModeControlSystem : public System {
  public:
    ModeControlSystem() {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;

        auto &mouse_position = inputs.getMousePosition();

        renderer.draw({renderer.width - 160, 0}, "Panel");

        if (mouse_position.x > renderer.width - 160) {
        } else {
            renderer.draw(inputs.getMousePosition(), "Cursor1");
        }
    }
};

class FiringModeSystem : public System {
  public:
    FiringModeSystem() {
        componentsNames.push_back("Turret");
        componentsNames.push_back("RotatableSprite");
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(const float dt, Entity &entity) override {
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();
        auto &rotatable = entity.get<RotatableSprite>();

        if (Context::get().inputs.isMousePressed(1)) {
            turret.mustFire = true;
        }

        {
            auto delta = cursor_position - entity.position;
            rotatable.angle = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;
        }
    }
};
