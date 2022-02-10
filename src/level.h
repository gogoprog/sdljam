#pragma once

#include "renderer.h"
#include "types.h"

using Path = Vector<Vector2>;

class Level {
  public:
    inline static int tileSpacing = Tile::SIZE * 2;

    Level();

    inline Vector2 getTileCoords(const Vector2 &world_pos) const {
        int col = int(std::floor(world_pos.x / tileSpacing));
        int row = int(std::floor(world_pos.y / tileSpacing));

        return Vector2(col, row);
    }

    inline Vector2 getTilePosition(const Vector2 &coords) const {
        return {coords.x * tileSpacing, coords.y * tileSpacing};
    }

    inline Vector2 getTileCenterPosition(const Vector2 &coords) const {
        return {coords.x * tileSpacing + tileSpacing / 2, coords.y * tileSpacing + tileSpacing / 2};
    }

    void setRoad(const Vector2 &coords, const bool value) {
        if (locks[coords])
            return;
        roadmap[coords] = value;
        updateCache({coords.x - 1, coords.y - 1}, {coords.x + 1, coords.y + 1});
    }

    void lock(const Vector2 &coords) {
        locks[coords] = true;
    }

    void lock2x2(const Vector2 &coords) {
        locks[coords] = true;
        locks[{coords.x - 1, coords.y}] = true;
        locks[{coords.x - 1, coords.y - 1}] = true;
        locks[{coords.x, coords.y - 1}] = true;
    }

    bool getRoad(const Vector2 &coords) {
        return roadmap[coords];
    }

    void render(Renderer &renderer);

    bool findPath(Path &path, const Vector2 &start, const Vector2 &end);

    bool isFree(const Vector2 &coords);
    bool canBuildAt(const Vector2 &coords);

    int width;
    int height;
    int tilewidth;
    int tileheight;

    Vector2 beginCoords;
    Vector2 endCoords;

  private:
    void buildCache();
    void updateCache(const Vector2 &from, const Vector2 &to);
    void setLockedRoad(const Vector2 &coords);

    Map<Vector2, bool> roadmap;
    Map<Vector2, bool> locks;
    Vector<int> cachedTypes;
};
