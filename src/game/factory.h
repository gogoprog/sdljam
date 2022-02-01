#pragma once

#include "../ecs.h"
#include "../types.h"

class Factory {
  public:
    static SharedPtr<Entity> createCamera();
    static SharedPtr<Entity> createBase();
    static SharedPtr<Entity> createTurret();
    static SharedPtr<Entity> createBullet();
    static SharedPtr<Entity> createVehicle();
    static SharedPtr<Entity> createSpawn();
    static SharedPtr<Entity> createSmallExplosion();
    static SharedPtr<Entity> createExplosion();
};
