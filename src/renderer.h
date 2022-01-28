#pragma once

#include "maths.h"
#include <memory>
#include <string>

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void clear();
    void update();

    void loadAtlas(const std::string &name);

    void draw(const Vector2 &pos, const std::string &name, const int frameindex, const bool use_pivot = true);

    void setPivot(const std::string &name, const int frameindex, const Vector2 &pivot);
    int getFramesCount(const std::string &name) const;
    void exportAtlas(const std::string &name);

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
