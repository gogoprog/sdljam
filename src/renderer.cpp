#include "renderer.h"
#include "maths.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>

struct Frame {
    SDL_Rect rect;
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

    atlas.texture = SDL_CreateTextureFromSurface(pimpl->renderer, atlas.surface);

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

    enum Result { WRONG_UP, WRONG_DOWN, WRONG_LEFT, WRONG_RIGHT, VALID };

    auto check_rect = [&](const SDL_Rect rect) {
        {
            int y = rect.y;
            for (int x = 0; x < surface->w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_UP;
            }
        }
        {
            int x = rect.x;
            for (int y = 0; y < surface->h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_LEFT;
            }
        }
        {
            int y = rect.y + rect.h;
            for (int x = 0; x < surface->w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_DOWN;
            }
        }
        {
            int x = rect.x + rect.w;
            for (int y = 0; y < surface->h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254)
                    return WRONG_RIGHT;
            }
        }

        return VALID;
    };

    for (int y = 0; y < surface->h; ++y) {
        for (int x = 0; x < surface->w; ++x) {
            auto v = pixels[y * pitch + x];

            if (v == 254) {
                SDL_Rect rect{x, y, x, y};
                rect.w = get_next_x(x, y) - rect.x;
                rect.h = get_next_y(x, y) - rect.y;

                auto result = check_rect(rect);

                if (result == WRONG_LEFT || result == WRONG_UP) {
                    continue;
                }

                while (result != VALID) {
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

                atlas.frames.push_back({rect});
            }
        }
    }

    pimpl->atlases[name] = atlas;
    std::cout << "Loaded " << atlas.frames.size() << " frames for atlas '" << name << "'" << std::endl;
}

void Renderer::draw(const Vector2 &pos, const std::string &name, const int frameindex) {
    auto &atlas = pimpl->atlases[name];
    auto rect = atlas.frames[frameindex].rect;
    auto drect = rect;
    drect.x = pos.x;
    drect.y = pos.y;
    drect.w *= 2;
    drect.h *= 2;
    SDL_RenderCopy(pimpl->renderer, atlas.texture, &rect, &drect);
}
