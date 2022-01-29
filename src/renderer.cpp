#include "renderer.h"
#include "maths.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>

struct Frame {
    SDL_Rect rect;
    Vector2 pivot;
};

struct Atlas {
    std::vector<Frame> frames;
    SDL_Surface *surface;
    SDL_Texture *texture;
};

struct Renderer::Pimpl {
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::map<std::string, Atlas> atlases;
    std::map<std::string, Terrain> terrains;
};

Renderer::Renderer() : pimpl(new Renderer::Pimpl()) {
}

Renderer::~Renderer() = default;

void Renderer::init() {
    SDL_CreateWindowAndRenderer(800, 600, SDL_WINDOW_OPENGL, &pimpl->window, &pimpl->renderer);
    SDL_SetWindowTitle(pimpl->window, "hardvacuum-defense");
}

void Renderer::clear() {
    SDL_RenderClear(pimpl->renderer);
}

void Renderer::update() {
    SDL_RenderPresent(pimpl->renderer);
}

void Renderer::loadAtlas(const std::string &name) {
    std::string path;
    path = "res/" + name + ".bmp";

    Atlas atlas;
    auto surface = SDL_LoadBMP(path.c_str());
    atlas.surface = surface;

    auto pixels = (uint8_t *)surface->pixels;
    auto pitch = surface->pitch;

    auto get_next_x = [&](const int inputx, const int inputy) {
        for (int x = inputx + 1; x < surface->w; ++x) {
            auto v = pixels[inputy * pitch + x];

            if (v == 254)
                return x;
        }
        return 0;
    };

    auto get_next_y = [&](const int inputx, const int inputy) {
        for (int y = inputy + 1; y < surface->h; ++y) {
            auto v = pixels[y * pitch + inputx];

            if (v == 254)
                return y;
        }
        return 0;
    };

    enum Result { WRONG_UP, WRONG_DOWN, WRONG_LEFT, WRONG_RIGHT, DEGENERATE, EMPTY, VALID };

    auto check_rect = [&](const SDL_Rect rect) {
        if (rect.w <= 0 || rect.h <= 0)
            return DEGENERATE;

        {
            int y = rect.y;
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_UP;
            }
        }
        {
            int x = rect.x;
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_LEFT;
            }
        }
        {
            int y = rect.y + rect.h;
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_DOWN;
            }
        }
        {
            int x = rect.x + rect.w;
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_RIGHT;
            }
        }

        for (int y = rect.y; y < rect.y + rect.h; ++y) {
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return VALID;
            }
        }

        return EMPTY;
    };

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            auto v = pixels[y * pitch + x];

            if (v == 254) {
                pixels[y * pitch + x] = 255;
                SDL_Rect rect{x, y, x, y};
                rect.w = get_next_x(x, y) - rect.x;
                rect.h = get_next_y(x, y) - rect.y;

                auto result = check_rect(rect);

                if (result == WRONG_LEFT || result == WRONG_UP) {
                    continue;
                }

                while (result != VALID && result != DEGENERATE && result != EMPTY) {
                    switch (result) {
                        case WRONG_RIGHT:
                            rect.w = get_next_x(rect.x + rect.w, y) - rect.x;
                            break;
                        case WRONG_DOWN:
                            rect.h = get_next_y(x, rect.y + rect.h) - rect.y;
                            break;
                        default:
                            break;
                    }

                    result = check_rect(rect);
                }

                if (result == VALID) {
                    atlas.frames.push_back({rect});
                }
            }
        }
    }

    SDL_SetColorKey(surface, true, 255);
    atlas.texture = SDL_CreateTextureFromSurface(pimpl->renderer, atlas.surface);

    pimpl->atlases[name] = atlas;
    std::cout << "Loaded " << atlas.frames.size() << " frames for atlas '" << name << "'" << std::endl;
}

void Renderer::loadTerrain(const std::string &name) {

    std::string path;
    path = "res/" + name + ".bmp";

    Terrain terrain;
    auto surface = SDL_LoadBMP(path.c_str());
    terrain.surface = surface;

    for (int y = 0; y < 9; ++y) {
        if (!(y & 1)) {

            for (int x = 0; x < 5; ++x) {
                if (!(x & 1)) {
                    terrain.tiles.push_back({x * Tile::SIZE, 1 + y * Tile::SIZE, Tile::SIZE, Tile::SIZE});
                }
            }
        }
    }

    SDL_SetColorKey(surface, true, 255);
    terrain.texture = SDL_CreateTextureFromSurface(pimpl->renderer, terrain.surface);

    pimpl->terrains[name] = terrain;
    std::cout << "Loaded terrain '" << name << "'" << std::endl;
}

const Terrain &Renderer::getTerrain(const std::string &name) {
    return pimpl->terrains[name];
}

void Renderer::draw(const Vector2 &pos, const std::string &name, const int frameindex, const bool use_pivot) {
    auto &atlas = pimpl->atlases[name];
    auto &frame = atlas.frames[frameindex];
    auto rect = frame.rect;
    auto drect = rect;
    drect.x = pos.x;
    drect.y = pos.y;
    if (use_pivot) {
        drect.x -= frame.pivot.x;
        drect.y -= frame.pivot.y;
    }
    drect.w *= 2;
    drect.h *= 2;
    SDL_RenderCopy(pimpl->renderer, atlas.texture, &rect, &drect);
}

void Renderer::draw(const Vector2 &pos, const Terrain &terrain, const int tileindex) {
    auto &frame = terrain.tiles[tileindex];
    auto rect = frame.rect;
    auto drect = rect;
    drect.x = pos.x;
    drect.y = pos.y;
    drect.w *= 2;
    drect.h *= 2;
    SDL_RenderCopy(pimpl->renderer, terrain.texture, &rect, &drect);
}

void Renderer::setPivot(const std::string &name, const int frameindex, const Vector2 &pivot) {
    auto &atlas = pimpl->atlases[name];
    auto &frame = atlas.frames[frameindex];
    frame.pivot = pivot;
}

int Renderer::getFramesCount(const std::string &name) const {
    return pimpl->atlases[name].frames.size();
}

void Renderer::exportAtlas(const std::string &name) {
    auto &atlas = pimpl->atlases[name];

    for (int i = 0; i < atlas.frames.size(); ++i) {
        auto &frame = atlas.frames[i];
        std::cout << "renderer.setPivot(\"" << name << "\", " << i << ", {" << frame.pivot.x << "," << frame.pivot.y
                  << "});" << std::endl;
    }
}
