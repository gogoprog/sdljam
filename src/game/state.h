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

    void onAdded() {
        time = 0;
    }

    void update(const float dt) override {
        System::update(dt);

        auto &inputs = Context::get().inputs;
        auto &renderer = Context::get().renderer;
        auto &mouse_position = inputs.getMousePosition();

        time += dt;

        if (mouse_position.x > renderer.width - 160) {
        } else {
            renderer.draw(inputs.getMousePosition(), "Cursor1");
        }

        if (time < 2.0) {
            renderer.drawText({128, 28}, "step 3:", 1);
            renderer.drawText({128, 70}, "fight!", 1);
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

  private:
    float time;
};

class RoadBuildingStateSystem : public System {
  public:
    RoadBuildingStateSystem() {
    }

    void onAdded() override {
        timeLeft = 10;
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
            if (inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, true);
            }
            renderer.draw(position, terrain, Tile::FILL1);
        } else {

            if (inputs.isMousePressed(1)) {
                level.setRoad(tile_coords, false);
            }
            renderer.draw(position, terrain, Tile::FILL2);
        }

        renderer.drawText({128, 28}, "step 1:", 1);
        renderer.drawText({128, 70}, "connect the roads!", 1);
        renderer.drawText({128, 110}, "click to build", 1);
        renderer.drawText({128, 150}, "hold shift to destroy", 1);

        timeLeft -= dt;

        Path path;
        auto correct = level.findPath(path, level.beginCoords, level.endCoords);

        if (correct) {
            String msg;
            msg = "good! starting in ";
            msg += std::to_string(int(timeLeft));
            msg += "s";
            renderer.drawText({128, 256}, msg.c_str(), 1);
        }

        if (timeLeft < 0 || inputs.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
            if (correct) {
                Context::get().game.changeState(Game::State::BUILDING_TURRETS);
            }
        }
    }

  private:
    float timeLeft;
};

class BuildingTurretsStateSystem : public System {
  public:
    BuildingTurretsStateSystem() {
    }

    void onAdded() override {
        timeLeft = 25;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;
        auto &game = Context::get().game;
        auto &atlas = renderer.getAtlas("Turret");
        auto world_position = Context::get().getMouseWorldPosition();

        auto tile_coords = level.getTileCoords(world_position);
        auto position = level.getTilePosition(tile_coords);

        timeLeft -= dt;

        if (timeLeft < 0 || inputs.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
            game.changeState(Game::State::PLAYING);
        } else {
            String msg;
            msg = "next wave in ";
            msg += std::to_string(int(timeLeft));
            msg += "s";
            renderer.drawText({128, 256}, msg.c_str(), 1);

            if (level.canBuildAt(tile_coords)) {

                renderer.draw(position, atlas, 0);

                if (inputs.isMouseJustPressed(1)) {
                    if (game.stats.money >= game.turretCost) {
                        {
                            auto e = Factory::createBase();
                            e->position = position;
                            engine->addEntity(e);
                        }

                        {
                            auto e = Factory::createTurret();
                            e->position = position;
                            engine->addEntity(e);
                        }

                        level.lock2x2(tile_coords);
                        game.stats.money -= game.turretCost;
                    }
                }

                auto pos = inputs.getMousePosition();
                pos.y += 32;
                renderer.drawText(pos, "build (" + std::to_string(game.turretCost) + ")");
            } else {
                Entity *hover_entity{nullptr};

                engine->iterate<Turret>([&](auto &e) {
                    auto delta = e.position - Vector2{position.x + 32, position.y + 32};
                    auto length = delta.getLength();

                    if (length < 64) {
                        hover_entity = &e;
                    }

                    return true;
                });

                if (hover_entity != nullptr) {
                    auto pos = inputs.getMousePosition();
                    pos.y += 32;

                    auto &turret = hover_entity->get<Turret>();
                    auto cost = 400 + turret.level * 200;
                    renderer.drawText(pos, "upgrade (" + std::to_string(cost) + ")");

                    if (inputs.isMouseJustPressed(1)) {

                        if (turret.level < 9) {
                            if (game.stats.money >= cost) {

                                game.stats.money -= cost;

                                turret.level++;
                            }
                        }
                    }
                }
            }
        }
        renderer.drawText({128, 28}, "step 2:", 1);
        renderer.drawText({128, 70}, "build or upgrade the turrets", 1);
    }

  private:
    float timeLeft;
};

class WinningStateSystem : public System {
  public:
    WinningStateSystem() {
    }

    void onAdded() override {
        timeLeft = 4;

        auto &game = Context::get().game;

        game.stats.money += 100 + game.waveCount * 100;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;

        timeLeft -= dt;

        if (timeLeft < 2) {
            renderer.drawText({256, 256}, "next wave!", 2);
        } else {
            renderer.drawText({256, 256}, "mission accomplished", 2);
        }

        if (timeLeft < 0) {
            Context::get().game.nextWave();
            Context::get().game.changeState(Game::State::BUILDING_ROADS);
        }
    }

  private:
    float timeLeft;
};

class LosingStateSystem : public System {
  public:
    LosingStateSystem() {
    }

    void onAdded() override {
        timeLeft = 2;
    }

    void update(const float dt) override {
        auto &inputs = Context::get().inputs;
        auto &level = Context::get().level;
        auto &renderer = Context::get().renderer;

        renderer.drawText({256, 256}, "you loser", 2);

        timeLeft -= dt;
        if (timeLeft < 0) {
            Context::get().game.reset();
        }
    }

  private:
    float timeLeft;
};
