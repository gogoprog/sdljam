#include "level.h"

void Level::render(Renderer &renderer) {
    auto &terrain = renderer.getTerrain("Snow");
    auto spacing = Tile::SIZE * 2;

    for (int i = 0; i < 64; ++i) {
        for (int j = 0; j < 64; ++j) {
            renderer.draw({i * spacing, j * spacing}, terrain, Tile::C);
        }
    }
}
