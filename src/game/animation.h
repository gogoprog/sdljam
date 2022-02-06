#pragma once

#include "../context.h"

#include "sprite.h"

struct Animation : public Component {
    inline static String name = "Animation";
    float frameRate;
    float time{0};
    bool loop{false};
    bool autoRemove{false};
};

class AnimationSystem : public System {
  public:
    AnimationSystem() {
        require<Animation>();
        require<Sprite>();
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &animation = entity.get<Animation>();
        auto &sprite = entity.get<Sprite>();
        auto &renderer = Context::get().renderer;
        int frame_count = renderer.getFramesCount(sprite.atlasName);

        animation.time += dt;

        auto duration = frame_count / animation.frameRate;

        if (animation.loop) {
            while (animation.time > duration) {
                animation.time -= duration;
            }
        }

        if (animation.time < duration) {
            int frame = int(animation.time * animation.frameRate) % frame_count;

            sprite.frameIndex = frame;
        } else {
            sprite.frameIndex = frame_count - 1;

            if (animation.autoRemove) {
                engine->removeEntity(entity);
            }
        }
    }
};
