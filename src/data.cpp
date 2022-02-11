#include "audio.h"
#include "game/turret.h"
#include "renderer.h"

Array<TurretStat, 10> turretStats;

void loadData(Renderer &renderer, Audio &audio) {
    renderer.loadFont("Font");

    renderer.loadAtlas("Turret");
    renderer.setPivot("Turret", 0, {43, 21});
    renderer.setPivot("Turret", 1, {43, 21});
    renderer.setPivot("Turret", 2, {18, 57});
    renderer.setPivot("Turret", 3, {20, 61});
    renderer.setPivot("Turret", 4, {58, 61});
    renderer.setPivot("Turret", 5, {25, 23});
    renderer.setPivot("Turret", 6, {20, 24});
    renderer.setPivot("Turret", 7, {18, 29});
    renderer.setPivot("Turret", 8, {57, 28});
    renderer.setPivot("Turret", 9, {54, 25});
    renderer.setPivot("Turret", 10, {18, 59});
    renderer.setPivot("Turret", 11, {20, 60});
    renderer.setPivot("Turret", 12, {58, 61});
    renderer.setPivot("Turret", 13, {26, 22});
    renderer.setPivot("Turret", 14, {22, 25});
    renderer.setPivot("Turret", 15, {18, 30});
    renderer.setPivot("Turret", 16, {58, 31});
    renderer.setPivot("Turret", 17, {53, 23});

    renderer.loadAtlas("Bullets", false);
    renderer.loadAtlas("Tank11", false);
    renderer.loadAtlas("exploBig", false, 1, false, true);
    renderer.loadAtlas("expSmall", false, 1, false, true);
    renderer.loadAtlas("explosn", false, 7, false, true, 20);
    renderer.loadAtlas("Flag", 20, 20, 5);

    renderer.loadTerrain("StoneSnow");
    renderer.loadTerrain("SnowPink");

    renderer.loadTexture("Panel", false);
    renderer.loadTexture("Cursor1");

    audio.loadSound("firing");
    audio.loadSound("impact");
    audio.loadSound("explosion");

    turretStats[0].range = 300;
    turretStats[0].damage = 50;
    turretStats[0].interval = 1;

    turretStats[1].range = 350;
    turretStats[1].damage = 60;
    turretStats[1].interval = 0.7;

    turretStats[2].range = 400;
    turretStats[2].damage = 60;
    turretStats[2].interval = 0.65;

    turretStats[3].range = 450;
    turretStats[3].damage = 70;
    turretStats[3].interval = 0.6;

    turretStats[4].range = 500;
    turretStats[4].damage = 75;
    turretStats[4].interval = 0.5;

    turretStats[5].range = 550;
    turretStats[5].damage = 75;
    turretStats[5].interval = 0.4;

    turretStats[6].range = 600;
    turretStats[6].damage = 75;
    turretStats[6].interval = 0.3;

    turretStats[7].range = 650;
    turretStats[7].damage = 75;
    turretStats[7].interval = 0.2;

    turretStats[8].range = 650;
    turretStats[8].damage = 80;
    turretStats[8].interval = 0.15;

    turretStats[9].range = 650;
    turretStats[9].damage = 100;
    turretStats[9].interval = 0.1;
}
