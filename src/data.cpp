#include "renderer.h"

void loadData(Renderer &renderer) {
    renderer.loadAtlas("Turret");
    renderer.setPivot("Turret", 0, {0, 0});
    renderer.setPivot("Turret", 1, {0, 0});
    renderer.setPivot("Turret", 2, {20, 69});
    renderer.setPivot("Turret", 3, {15, 67});
    renderer.setPivot("Turret", 4, {65, 66});
    renderer.setPivot("Turret", 5, {14, 23});
    renderer.setPivot("Turret", 6, {13, 18});
    renderer.setPivot("Turret", 7, {17, 18});
    renderer.setPivot("Turret", 8, {62, 21});
    renderer.setPivot("Turret", 9, {63, 23});
    renderer.setPivot("Turret", 10, {20, 69});
    renderer.setPivot("Turret", 11, {0, 0});
    renderer.setPivot("Turret", 12, {0, 0});
    renderer.setPivot("Turret", 13, {0, 0});
    renderer.setPivot("Turret", 14, {0, 0});
    renderer.setPivot("Turret", 15, {0, 0});
    renderer.setPivot("Turret", 16, {0, 0});
    renderer.setPivot("Turret", 17, {0, 0});
}
