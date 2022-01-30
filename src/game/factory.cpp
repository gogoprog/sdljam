#include "factory.h"

#include "camera.h"
#include "control.h"
#include "sprite.h"
#include "turret.h"
#include "ui.h"
#include "bullet.h"

SharedPtr<Entity> Factory::createCamera() {
    auto e = std::make_shared<Entity>();
    e->add<Camera>();
    e->add<Control>();
    e->position = {0, 0};

    return e;
}

SharedPtr<Entity> Factory::createBase() {
    auto e = std::make_shared<Entity>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Turret";
    e->get<Sprite>().frameIndex = 0;

    return e;
}

SharedPtr<Entity> Factory::createTurret() {
    auto e = std::make_shared<Entity>();
    e->add<Turret>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Turret";
    e->get<Sprite>().frameIndex = 2;
    e->add<RotatableSprite>();
    e->get<RotatableSprite>().frames = {2, 3, 5, 6, 7, 8, 9, 4};

    return e;
}

SharedPtr<Entity> Factory::createBullet() {
    auto e = std::make_shared<Entity>();
    e->add<Bullet>();
    e->add<Sprite>();
    e->get<Sprite>().atlasName = "Bullets";
    e->get<Sprite>().frameIndex = 0;
    e->get<Sprite>().layer = 2;

    return e;
}
