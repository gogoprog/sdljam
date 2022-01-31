#include "level.h"
#include <queue>

Level::Level() {
    tilewidth = 64;
    tileheight = 64;

    width = tilewidth * tileSpacing;
    height = tileheight * tileSpacing;

    buildCache();

    for (int i = 0; i < 50; ++i) {
        setRoad({20, i}, true);
    }

    for (int i = 20; i < 64; ++i) {
        setRoad({i, 50}, true);
    }

    beginCoords = {64, 50};
    endCoords = {20, 0};

    /* setRoad({20, 1}, false); */
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

bool Level::findPath(Path &path, const Vector2 &start, const Vector2 &end) {
    struct Node {
        Path path;
        float distanceLeft;

        bool operator<(const Node &other) const {
            if (distanceLeft == other.distanceLeft) {
                return path.size() > other.path.size();
            }
            return distanceLeft > other.distanceLeft;
        }
    };

    std::priority_queue<Node> q;
    q.push({{start}, (start - end).getLength()});

    Map<Vector2, int> visited;

    while (!q.empty()) {
        const auto node = q.top();
        q.pop();
        const auto last = node.path.back();

        if (visited.find(last) == visited.end() || visited[last] > node.path.size()) {
            visited[last] = node.path.size();

            if (last == end) {
                path = node.path;
                return true;
                break;
            } else {
                for (auto &direction : directions) {
                    auto next = last + direction;

                    if (getRoad(next)) {
                        if (std::find(node.path.begin(), node.path.end(), next) == node.path.end()) {
                            auto copy = node;
                            copy.path.push_back(next);
                            copy.distanceLeft = (next - end).getLength();
                            q.push(copy);
                        }
                    }
                }
            }
        }
    }

    return false;
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
