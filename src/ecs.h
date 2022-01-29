#pragma once

#include "types.h"
#include <algorithm>
#include <memory>
#include <vector>

struct Component {
    Component(const String &_name) : name(_name){};
    String name;
};

class Engine;

class Entity {
    friend class Engine;

  public:
    String name;
    Vector2 position;

    template <typename T> void add() {
        components.push_back(std::make_unique<T>());

        notifyAdd(components.back()->name);
    }

    void notifyAdd(const String component_name);
    void notifyRemove(const String component_name);

  private:
    Vector<UniquePtr<Component>> components;
    Engine *engine{nullptr};
};

class System {
    friend class Engine;

  public:
    System() = default;
    ~System() = default;

    virtual void onEntityAdded(Entity &entity){};
    virtual void updateSingle(Entity &entity) = 0;

  protected:
    Vector<String> componentsNames;
};

class Engine {
  public:
    void addSystem(System *system) {
        systems.push_back(system);
    };

    void addEntity(SharedPtr<Entity> entity) {
        entities.push_back(entity);
        entity->engine = this;

        for (auto &component : entity->components) {
            notifyAdd(*entity, component->name);
        }
    }

    void update() {
        for (auto &system : systems) {
            for (auto &entityptr : entities) {
                auto &entity = *entityptr;
                if (std::all_of(system->componentsNames.begin(), system->componentsNames.end(), [&](auto name) {
                        auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                               [&](auto &component) { return component->name == name; });
                        return it != entity.components.end();
                    })) {
                    system->updateSingle(entity);
                }
            }
        }
    }

    void notifyAdd(Entity &entity, const String component_name) {

        for (auto &system : systems) {

            if (std::find(system->componentsNames.begin(), system->componentsNames.end(), component_name) !=
                system->componentsNames.end()) {

                if (std::all_of(system->componentsNames.begin(), system->componentsNames.end(), [&](auto name) {
                        auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                               [&](auto &component) { return component->name == name; });
                        return it != entity.components.end();
                    })) {
                    system->onEntityAdded(entity);
                }
            }
        }
    }

  private:
    std::vector<System *> systems;
    std::vector<SharedPtr<Entity>> entities;
};

void Entity::notifyAdd(const String component_name) {
    if (engine) {
        engine->notifyAdd(*this, component_name);
    }
}

/* void Entity::notifyRemove(const String component_name); */
