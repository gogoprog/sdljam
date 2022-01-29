#pragma once

#include "context.h"

class TurretSystem : public System {
  public:
    TurretSystem() {
        componentsNames.push_back("Turret");
    }

    void onEntityAdded(Entity &entity) override {
    }

    void updateSingle(Entity &entity) override {
        Context::get().renderer.draw(entity.position, "Turret", 0);
        Context::get().renderer.draw(entity.position, "Turret", 10);
    }
};

struct Turret : public Component {
    Turret() : Component("Turret"){};
};

class Game {
  public:
    static void init() {
        Context::get().engine.addSystem(new TurretSystem());

        {
            auto e = std::make_shared<Entity>();
            e->add<Turret>();
            e->position = {128, 256};
            Context::get().engine.addEntity(e);
        }
        {
            auto e = std::make_shared<Entity>();
            e->add<Turret>();
            e->position = {512, 256};
            Context::get().engine.addEntity(e);
        }
    }
};
