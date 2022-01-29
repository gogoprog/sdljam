#include "renderer.h"

void loadData(Renderer &renderer) {
    renderer.loadAtlas("Turret");
    renderer.setPivot("Turret", 0, {43, 19});
    renderer.setPivot("Turret", 1, {43, 19});
    renderer.setPivot("Turret", 2, {20, 69});
    renderer.setPivot("Turret", 3, {15, 67});
    renderer.setPivot("Turret", 4, {65, 66});
    renderer.setPivot("Turret", 5, {14, 23});
    renderer.setPivot("Turret", 6, {13, 18});
    renderer.setPivot("Turret", 7, {17, 18});
    renderer.setPivot("Turret", 8, {62, 21});
    renderer.setPivot("Turret", 9, {63, 23});
    renderer.setPivot("Turret", 10, {20, 69});
    renderer.setPivot("Turret", 11, {13, 66});
    renderer.setPivot("Turret", 12, {65, 67});
    renderer.setPivot("Turret", 13, {11, 20});
    renderer.setPivot("Turret", 14, {12, 15});
    renderer.setPivot("Turret", 15, {15, 15});
    renderer.setPivot("Turret", 16, {63, 18});
    renderer.setPivot("Turret", 17, {66, 22});

    renderer.loadTerrain("Snow");
    renderer.loadTerrain("Snow2");
}
