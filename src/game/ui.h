#pragma once

#include "../context.h"

class UiSystem : public System {
  public:
    UiSystem() {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;

        auto &mouse_position = inputs.getMousePosition();

        /*         renderer.draw({renderer.width - 160, 0}, "Panel"); */

        /*         if (mouse_position.x > renderer.width - 160) { */
        /*         } else { */
        /*             renderer.draw(inputs.getMousePosition(), "Cursor1"); */
        /*         } */
    }
};
