#pragma once

#include "renderer.h"
#include "types.h"

class Level {
  public:
    Level();
    void render(Renderer &renderer);

    int width;
    int height;
    int tilewidth;
    int tileheight;

    inline static int tileSpacing = Tile::SIZE * 2;

    Vector2 getTileCoords(const Vector2 world_pos) const {
        int col = int(world_pos.x / tileSpacing);
        int row = int(world_pos.y / tileSpacing);

        return Vector2(col, row);
    }

    Vector2 getTilePosition(const Vector2 &coords) const {
        return {coords.x * tileSpacing, coords.y * tileSpacing};
    }

    void setRoad(const Vector2 &coords, const bool value) {
        roadmap[coords] = value;
        updateCache({coords.x - 1, coords.y - 1}, {coords.x + 1, coords.y + 1});
    }

    bool getRoad(const Vector2 &coords) {
        return roadmap[coords];
    }

    void buildCache();
    void updateCache(const Vector2 &from, const Vector2 &to);

  private:
    Map<Vector2, bool> roadmap;
    Vector<int> cachedTypes;
};
