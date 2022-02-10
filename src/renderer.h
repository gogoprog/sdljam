#pragma once

#include "types.h"
#include <SDL.h>
#include <memory>
#include <string>
#include <vector>

struct Tile {
    SDL_Rect rect;

    inline static const int SIZE = 20;
    enum Type {
        NW,
        N,
        NE,
        W,
        FILL,
        E,
        SW,
        S,
        SE,
        FILL1,
        CORNER1,
        CORNER2,
        FILL2,
        CORNER3,
        CORNER4,
    };
};

struct Terrain {
    std::vector<Tile> tiles;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

struct Frame {
    SDL_Rect rect;
    Vector2 pivot;
};

struct Atlas {
    Vector<Frame> frames;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

class Renderer {
  public:
    Renderer();
    ~Renderer();

    void init();
    void clear();
    void update();

    void loadAtlas(const std::string &name, const bool skip_empty = true, const int delimiter = 254,
                   const bool check_content = true, const bool skip1 = false, const int step = 1);
    void loadAtlas(const std::string &name, const int frame_width, const int frame_height, const int max_frames,
                   const bool skip1 = true);
    void loadTerrain(const std::string &name);
    void loadTexture(const std::string &name, const bool center_pivot = true);
    void loadFont(const std::string &name);

    const Terrain &getTerrain(const std::string &name);
    const Atlas &getAtlas(const std::string &name);

    void draw(const Vector2 &pos, const Atlas &atlas, const int frameindex, const bool use_pivot = true,
              const float scale = 1.0f, const bool use_camera = true);
    void draw(const Vector2 &pos, const std::string &name, const int frameindex, const bool use_pivot = true);
    void draw(const Vector2 &pos, const Terrain &terrain, const int tileindex);
    void draw(const Vector2 &pos, const std::string &name);

    void drawText(const Vector2 &pos, const std::string &text, const float scale = 1.0f, const bool background = true);

    void setPivot(const std::string &name, const int frameindex, const Vector2 &pivot);
    int getFramesCount(const std::string &name) const;
    void exportAtlas(const std::string &name);

    Vector2 &getCameraPosition();
    const Vector2 &getCameraPosition() const;

    void onWindowEvent(const SDL_WindowEvent &wevent);

    int width;
    int height;

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
