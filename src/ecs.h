#pragma once

#include "types.h"
#include <algorithm>

struct Component {};

class Engine;

class Entity {
    friend class Engine;

  public:
    String name;
    Vector2 position;

    template <typename T> void add() {
        components[T::name] = std::make_unique<T>();

        notifyAdd(T::name);
    }

    template<typename T>
    T & get() {
        return components[T::name];
    }

    void notifyAdd(const String component_name);
    void notifyRemove(const String component_name);

  private:
    Map<String, UniquePtr<Component>> components;
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

        for (auto &kv : entity->components) {
            notifyAdd(*entity, kv.first);
        }
    }

    void update() {
        for (auto &system : systems) {
            for (auto &entityptr : entities) {
                auto &entity = *entityptr;
                if (std::all_of(system->componentsNames.begin(), system->componentsNames.end(), [&](auto name) {
                        auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                               [&](auto &kv) { return kv.first == name; });
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
                                               [&](auto &kv) { return kv.first == name; });
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
