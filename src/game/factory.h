#pragma once

#include "../ecs.h"
#include "../types.h"

class Factory {
  public:
    static SharedPtr<Entity> createCamera();
    static SharedPtr<Entity> createBase();
    static SharedPtr<Entity> createTurret();
    static SharedPtr<Entity> createBullet();
};
