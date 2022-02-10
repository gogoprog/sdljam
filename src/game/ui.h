#pragma once

#include "../context.h"

class UiSystem : public System {
  public:
    UiSystem() {
        priority = 10;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        auto &mouse_position = inputs.getMousePosition();

        renderer.draw({renderer.width - 160, 0}, "Panel");

        {
            char buffer[64];
            sprintf(buffer, "%15d", game.stats.money);
            renderer.drawText({renderer.width - 130, 191}, "money", 1, false);
            renderer.drawText({renderer.width - 130, 218}, buffer, 1, false);
        }
        {
            char buffer[64];
            sprintf(buffer, "%15d", game.stats.lifes);
            renderer.drawText({renderer.width - 130, 188 + 150}, "lifes", 1, false);
            renderer.drawText({renderer.width - 130, 215 + 150}, buffer, 1, false);
        }
    }
};
