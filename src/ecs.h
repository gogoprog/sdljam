#pragma once

#include "types.h"
#include <algorithm>

struct Component {};

class Engine;
class System;

class Entity {
    friend class Engine;
    friend class System;

  public:
    String name;
    Vector2 position;

    template <typename T, typename... Args> void add(Args... args) {
        components[T::name] = std::make_unique<T>(args...);

        notifyAdd(T::name);
    }

    template <typename T> T &get() {
        return (T &)*components[T::name];
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

    virtual void onAdded(){};
    virtual void onRemoved(){};

    virtual void update(const float dt);
    virtual void onEntityAdded(Entity &entity){};
    virtual void updateSingle(const float dt, Entity &entity){};

  protected:
    Vector<String> componentsNames;
    Engine *engine{nullptr};
};

class Engine {
    friend class System;

  public:
    void addSystem(System *system) {
        systems.push_back(system);
        system->engine = this;
        system->onAdded();
    };

    void removeSystem(System *system) {
        auto it = std::find(systems.begin(), systems.end(), system);
        if (it != systems.end()) {
            systems.erase(std::find(systems.begin(), systems.end(), system));
            system->onRemoved();
        }
    };

    void addEntity(SharedPtr<Entity> entity) {
        entities.push_back(entity);
        entity->engine = this;

        for (auto &kv : entity->components) {
            notifyAdd(*entity, kv.first);
        }
    }

    void removeEntity(Entity &entity) {
        auto end_it = std::remove_if(entities.begin(), entities.end(), [&](auto &se) { return &*se == &entity; });
        entities.resize(end_it - entities.begin());
    }

    void update(const float dt) {
        auto systems_copy = systems;
        for (auto &system : systems_copy) {
            system->update(dt);
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

inline void Entity::notifyAdd(const String component_name) {
    if (engine) {
        engine->notifyAdd(*this, component_name);
    }
}

inline void System::update(const float dt) {
    auto entities = engine->entities;
    for (auto &entityptr : entities) {
        auto &entity = *entityptr;
        if (std::all_of(componentsNames.begin(), componentsNames.end(), [&](auto name) {
                auto it = std::find_if(entity.components.begin(), entity.components.end(),
                                       [&](auto &kv) { return kv.first == name; });
                return it != entity.components.end();
            })) {
            updateSingle(dt, entity);
        }
    }
}
