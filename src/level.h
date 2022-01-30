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

  private:
    Map<Vector2, bool> roadmap;
};
