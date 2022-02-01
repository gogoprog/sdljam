#include "renderer.h"

void loadData(Renderer &renderer) {
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

    renderer.loadTerrain("StoneSnow");
    renderer.loadTerrain("SnowPink");

    renderer.loadTexture("Panel", false);
    renderer.loadTexture("Cursor1");
}
