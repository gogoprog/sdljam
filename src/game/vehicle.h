#pragma once

#include "../context.h"
#include "move.h"

struct Vehicle : public Component {
    inline static String name = "Vehicle";
    Path path;
    float speed = 100;
};

class VehicleSystem : public System {
  public:
    VehicleSystem() {
        componentsNames.push_back("Vehicle");
        componentsNames.push_back("Movable");
    }

    void onEntityAdded(Entity &entity) override {
        /* auto &move = entity.add<Move>(entity.position, Vector2{350, 256}, 100); */
    }

    void updateSingle(const float dt, Entity &entity) override {
        auto &level = Context::get().level;
        auto &game = Context::get().game;
        auto &vehicle = entity.get<Vehicle>();

        if (!entity.has<Move>()) {

            vehicle.path.resize(0);

            auto start_coords = level.getTileCoords(entity.position);

            if (start_coords == level.endCoords) {
                game.stats.lifes--;

                Context::get().audio.playSound("error");
                if (game.stats.lifes == 0) {
                    game.changeState(Game::State::LOSING);
                }

                engine->removeEntity(entity);
            } else {
                if (level.findPath(vehicle.path, start_coords, level.endCoords)) {
                    if (vehicle.path.size() > 1) {
                        auto position = level.getTileCenterPosition(vehicle.path[1]);
                        entity.add<Move>(entity.position, position, vehicle.speed);
                    } else if (vehicle.path.size() == 0) {
                    }
                } else {
                    auto position = level.getTileCenterPosition(start_coords + directions[rand() % 8]);
                    entity.add<Move>(entity.position, position, vehicle.speed);
                }
            }
        }

        /* for (auto &p : vehicle.path) { */
        /*     auto pos = level.getTileCenterPosition(p); */
        /*     Context::get().renderer.draw(pos, "Bullets", 0); */
        /* } */
    }
};
