#pragma once

#include <memory>
#include <string>
#include "maths.h"

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void clear();
    void update();

    void loadAtlas(const std::string &name);

    void draw(const Vector2 &pos, const std::string &name, const int frameindex);

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
