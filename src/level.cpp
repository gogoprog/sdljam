#include "level.h"

Level::Level() {

    tilewidth = 256;
    tileheight = 256;
    width = tilewidth * tileSpacing;
    height = tileheight * tileSpacing;

    buildCache();

    for (int i = 0; i < 64; ++i) {
        setRoad({i, 14}, true);
        setRoad({20, i}, true);
    }
}

void Level::render(Renderer &renderer) {
    auto &terrain = renderer.getTerrain("StoneSnow");

    int i = 0;
    for (int y = 0; y < tileheight; ++y) {
        for (int x = 0; x < tilewidth; ++x) {
            renderer.draw({x * tileSpacing, y * tileSpacing}, terrain, cachedTypes[i]);
            ++i;
        }
    }
}

void Level::buildCache() {
    cachedTypes.resize(tilewidth * tileheight);
    updateCache({0, 0}, {tilewidth - 1, tileheight - 1});
}

void Level::updateCache(const Vector2 &from, const Vector2 &to) {
    auto minx = std::max<int>(from.x, 0);
    auto maxx = std::min<int>(to.x, tilewidth - 1);
    auto miny = std::max<int>(from.y, 0);
    auto maxy = std::min<int>(to.y, tileheight - 1);

    for (int x = minx; x <= maxx; ++x) {
        for (int y = miny; y <= maxy; ++y) {
            Vector2 pos{x, y};
            int type = Tile::FILL2;

            if (getRoad(pos)) {
                type = Tile::FILL1;
            } else {

                if (getRoad({x + 1, y + 1})) {
                    type = Tile::CORNER4;
                }
                if (getRoad({x - 1, y + 1})) {
                    type = Tile::CORNER3;
                }
                if (getRoad({x - 1, y - 1})) {
                    type = Tile::CORNER1;
                }
                if (getRoad({x + 1, y - 1})) {
                    type = Tile::CORNER2;
                }

                if (getRoad({x + 1, y})) {
                    type = Tile::E;

                    if (getRoad({x, y + 1})) {
                        type = Tile::SE;
                    } else if (getRoad({x, y - 1})) {
                        type = Tile::NE;
                    }
                } else if (getRoad({x - 1, y})) {
                    type = Tile::W;

                    if (getRoad({x, y + 1})) {
                        type = Tile::SW;
                    } else if (getRoad({x, y - 1})) {
                        type = Tile::NW;
                    }
                } else if (getRoad({x, y - 1})) {
                    type = Tile::N;
                } else if (getRoad({x, y + 1})) {
                    type = Tile::S;
                }
            }

            cachedTypes[y * tilewidth + x] = type;
        }
    }
}
