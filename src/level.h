#pragma once

#include "renderer.h"
#include "types.h"

class Level {
  public:
    Level();
    void render(Renderer &renderer);

  private:
    Map<Vector2, bool> roadmap;
};
