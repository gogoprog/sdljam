#include "level.h"

Level::Level() {

    for (int i = 0; i < 64; ++i) {
        roadmap[{i, 14}] = true;
        roadmap[{20, i}] = true;
    }
}

void Level::render(Renderer &renderer) {
    auto &terrain = renderer.getTerrain("StoneSnow");
    auto spacing = Tile::SIZE * 2;

    for (int x = 0; x < 64; ++x) {
        for (int y = 0; y < 64; ++y) {
            Vector2 pos{x, y};
            int type = Tile::FILL2;

            if (roadmap[pos]) {
                type = Tile::FILL1;
            } else {

                if (roadmap[{x + 1, y + 1}]) {
                    type = Tile::CORNER4;
                }
                if (roadmap[{x - 1, y + 1}]) {
                    type = Tile::CORNER3;
                }
                if (roadmap[{x - 1, y - 1}]) {
                    type = Tile::CORNER1;
                }
                if (roadmap[{x + 1, y - 1}]) {
                    type = Tile::CORNER2;
                }

                if (roadmap[{x + 1, y}]) {
                    type = Tile::E;

                    if (roadmap[{x, y + 1}]) {
                        type = Tile::SE;
                    } else if (roadmap[{x, y - 1}]) {
                        type = Tile::NE;
                    }
                } else if (roadmap[{x - 1, y}]) {
                    type = Tile::W;

                    if (roadmap[{x, y + 1}]) {
                        type = Tile::SW;
                    } else if (roadmap[{x, y - 1}]) {
                        type = Tile::NW;
                    }
                } else if (roadmap[{x, y - 1}]) {
                    type = Tile::N;
                } else if (roadmap[{x, y + 1}]) {
                    type = Tile::S;
                }
            }

            renderer.draw({x * spacing, y * spacing}, terrain, type);
        }
    }
}
