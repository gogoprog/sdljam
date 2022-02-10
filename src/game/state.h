#pragma once

#include "../context.h"
#include "sprite.h"
#include "turret.h"

class FiringStateSystem : public System {
  public:
    FiringStateSystem() {
        componentsNames.push_back("Turret");
        componentsNames.push_back("RotatableSprite");
    }

    void update(const float dt) override {
        System::update(dt);

        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &mouse_position = inputs.getMousePosition();

        if (mouse_position.x > renderer.width - 160) {
        } else {
            renderer.draw(inputs.getMousePosition(), "Cursor1");
        }
    }

    void updateSingle(const float dt, Entity &entity) override {
        const auto cursor_position = Context::get().getMouseWorldPosition();
        auto &turret = entity.get<Turret>();
        auto &rotatable = entity.get<RotatableSprite>();

        if (turret.timeSinceLastFire > 0.20 && Context::get().inputs.isMousePressed(1)) {
            turret.mustFire = true;
        }

        {
            auto delta = cursor_position - entity.position;
            rotatable.angle = (std::atan2(delta.y, delta.x) * 180.0f / std::numbers::pi) + 90;
        }
    }
};

class RoadBuildingStateSystem : public System {
  public:
    RoadBuildingStateSystem() {
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        auto &terrain = renderer.getTerrain("StoneSnow");

        if (!inputs.isKeyPressed(SDL_SCANCODE_LSHIFT)) {
            if (Context::get().inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, true);
            }
            renderer.draw(position, terrain, Tile::FILL1);
        } else {

            if (Context::get().inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, false);
            }
            renderer.draw(position, terrain, Tile::FILL2);
        }

        renderer.drawText({128, 28}, "step 1:", 1);
        renderer.drawText({128, 70}, "connect the roads", 1);

        Path path;
        if (level.findPath(path, level.beginCoords, level.endCoords)) {
            Context::get().game.changeState(Game::State::BUILDING_TURRETS);
        }
    }

  private:
    SharedPtr<Entity> preview;
};
