#include "renderer.h"

#include "types.h"
#include <SDL.h>
#include <iostream>
#include <map>
#include <vector>

struct Texture {
    SDL_Rect rect;
    SDL_Texture *texture;
    Vector2 pivot;
};

struct Renderer::Pimpl {
    SDL_Window *window;
    SDL_Renderer *renderer;
    std::map<String, Atlas> atlases;
    std::map<String, Terrain> terrains;
    std::map<String, Texture> textures;
    Vector2 cameraPosition{0, 0};
};

Renderer::Renderer() : pimpl(new Renderer::Pimpl()) {
}

Renderer::~Renderer() = default;

void Renderer::init() {
    width = 1280;
    height = 800;
    SDL_CreateWindowAndRenderer(width, height, SDL_WINDOW_OPENGL, &pimpl->window, &pimpl->renderer);
    SDL_SetWindowTitle(pimpl->window, "hardvacuum-defense");
}

void Renderer::clear() {
    SDL_RenderClear(pimpl->renderer);
}

void Renderer::update() {
    SDL_RenderPresent(pimpl->renderer);
}

void Renderer::loadAtlas(const std::string &name, const bool skip_empty, const int delimiter, const bool check_content,
                         const bool skip1, const int step) {
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

            if (v == delimiter)
                return x;
        }
        return 0;
    };

    auto get_next_y = [&](const int inputx, const int inputy) {
        for (int y = inputy + 1; y < surface->h; ++y) {
            auto v = pixels[y * pitch + inputx];

            if (v == delimiter)
                return y;
        }
        return 0;
    };

    enum Result { WRONG_UP, WRONG_DOWN, WRONG_LEFT, WRONG_RIGHT, DEGENERATE, EMPTY, VALID };

    auto check_rect = [&](const SDL_Rect rect) {
        if (rect.w <= 0 || rect.h <= 0)
            return DEGENERATE;

        if (!check_content)
            return VALID;

        {
            int y = rect.y;
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254 && v != delimiter)
                    return WRONG_UP;
            }
        }
        {
            int x = rect.x;
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254 && v != delimiter)
                    return WRONG_LEFT;
            }
        }
        {
            int y = rect.y + rect.h;
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254 && v != delimiter)
                    return WRONG_DOWN;
            }
        }
        {
            int x = rect.x + rect.w;
            for (int y = rect.y; y < rect.y + rect.h; ++y) {
                auto v = pixels[y * pitch + x];

                if (v < 254 && v != delimiter)
                    return WRONG_RIGHT;
            }
        }

        for (int y = rect.y; y < rect.y + rect.h; ++y) {
            for (int x = rect.x; x < rect.x + rect.w; ++x) {
                auto v = pixels[y * pitch + x];

                if (v < 254 && v != delimiter)
                    return VALID;
            }
        }

        return EMPTY;
    };

    for (int y = 0; y < surface->h; y += step) {
        for (int x = 0; x < surface->w; x += step) {
            auto v = pixels[y * pitch + x];

            if (v == delimiter) {
                pixels[y * pitch + x] = 255;
                SDL_Rect rect{x, y, x, y};

                if (skip1) {
                    rect.x++;
                    rect.y++;
                }

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

                if (result == VALID || (!skip_empty && result == EMPTY)) {
                    atlas.frames.push_back({rect, {rect.w, rect.h}});
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
    std::cout << "Loaded " << terrain.tiles.size() << " tiles for terrain '" << name << "'" << std::endl;
}

void Renderer::loadTexture(const std::string &name, const bool center_pivot) {
    std::string path;
    path = "res/" + name + ".bmp";
    auto surface = SDL_LoadBMP(path.c_str());
    Texture texture;
    SDL_SetColorKey(surface, true, 255);
    texture.texture = SDL_CreateTextureFromSurface(pimpl->renderer, surface);
    texture.rect = {0, 0, surface->w, surface->h};

    if (center_pivot) {
        texture.pivot = {surface->w, surface->h};
    } else {
        texture.pivot = {0, 0};
    }

    pimpl->textures[name] = texture;
    std::cout << "Loaded texture '" << name << "'" << std::endl;
}

void Renderer::loadFont(const std::string &name) {
    std::string path;
    path = "res/" + name + ".bmp";
    auto surface = SDL_LoadBMP(path.c_str());
    Atlas atlas;

    atlas.surface = surface;
    auto pixels = (uint8_t *)surface->pixels;
    auto pitch = surface->pitch;

    auto isValid = [&](const int x, const int fromy) {
        for (int y = fromy; y <= fromy + 10; ++y) {
            auto v = pixels[y * pitch + x];

            if (v != 36 && v > 0) {
                return false;
            }
        }

        return true;
    };

    auto findNextX = [&](const int fromx, const int y, const int max_x = 0) {
        auto max = max_x != 0 ? max_x : surface->w;

        for (int x = fromx + 1; x < max; x++) {
            if (isValid(x, y)) {
                return x;
            }
        }

        return max_x == 0 ? -1 : max_x - 1;
    };

    auto y = 24;

    for (int x = 7; x < surface->w; x += 1) {
        auto x2 = findNextX(x, y);

        if (x2 != -1) {
            SDL_Rect rect = {x, y, x2 - x + 1, 11};
            atlas.frames.push_back({rect});
            x = x2 + 1;
        } else {
            break;
        }
    }

    y = 12;

    auto count = 0;

    for (int x = 195; x < surface->w; x += 1) {
        auto x2 = findNextX(x, y, x + 6);

        if (x2 != -1) {
            SDL_Rect rect = {x, y, x2 - x + 1, 9};

            if (rect.w >= 3) {
                atlas.frames.push_back({rect});
                ++count;
                if (count >= 10) {
                    break;
                }
            }

            x = x2;
        } else {
            break;
        }
    }

    SDL_SetColorKey(surface, true, 36);
    atlas.texture = SDL_CreateTextureFromSurface(pimpl->renderer, atlas.surface);

    pimpl->atlases[name] = atlas;
    std::cout << "Loaded " << atlas.frames.size() << " frames for font '" << name << "'" << std::endl;
}

const Terrain &Renderer::getTerrain(const std::string &name) {
    return pimpl->terrains[name];
}

void Renderer::draw(const Vector2 &pos, const Atlas &atlas, const int frameindex, const bool use_pivot,
                    const float scale, const bool use_camera) {
    auto &frame = atlas.frames[frameindex];
    auto rect = frame.rect;
    auto drect = rect;

    drect.x = pos.x;
    drect.y = pos.y;

    if (use_camera) {
        drect.x -= pimpl->cameraPosition.x;
        drect.y -= pimpl->cameraPosition.y;
    }

    if (use_pivot) {
        drect.x -= frame.pivot.x;
        drect.y -= frame.pivot.y;
    }

    drect.w *= 2 * scale;
    drect.h *= 2 * scale;

    SDL_RenderCopy(pimpl->renderer, atlas.texture, &rect, &drect);
}

void Renderer::draw(const Vector2 &pos, const std::string &name, const int frameindex, const bool use_pivot) {
    auto &atlas = pimpl->atlases[name];
    draw(pos, atlas, frameindex, use_pivot);
}

void Renderer::draw(const Vector2 &pos, const Terrain &terrain, const int tileindex) {
    auto &frame = terrain.tiles[tileindex];
    auto rect = frame.rect;
    auto drect = rect;
    drect.x = pos.x - pimpl->cameraPosition.x;
    drect.y = pos.y - pimpl->cameraPosition.y;
    drect.w *= 2;
    drect.h *= 2;
    SDL_RenderCopy(pimpl->renderer, terrain.texture, &rect, &drect);
}

void Renderer::draw(const Vector2 &pos, const std::string &name) {
    auto &texture = pimpl->textures[name];
    auto rect = texture.rect;
    auto drect = rect;
    drect.x = pos.x;
    drect.y = pos.y;
    drect.x -= texture.pivot.x;
    drect.y -= texture.pivot.y;
    drect.w *= 2;
    drect.h *= 2;
    SDL_RenderCopy(pimpl->renderer, texture.texture, &rect, &drect);
}

void Renderer::drawText(const Vector2 &pos, const std::string &text) {
    auto &atlas = pimpl->atlases["Font"];
    auto current_pos = pos;

    for (auto c : text) {
        int frame_index = -1;

        if (c >= 'a' && c <= 'z') {
            frame_index = c - 'a';
        }

        if (c >= '0' && c <= '9') {
            frame_index = c - '0' + 18 + 26;
        }

        if (c == ' ') {
            current_pos.x += 5;
        }

        if (frame_index != -1) {
            auto &frame = atlas.frames[frame_index];
            draw(current_pos, atlas, frame_index, false, 2.0f, false);
            current_pos.x += 1 + frame.rect.w * 4;
        }
    }
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

Vector2 &Renderer::getCameraPosition() {
    return pimpl->cameraPosition;
}

const Vector2 &Renderer::getCameraPosition() const {
    return pimpl->cameraPosition;
}

void Renderer::onWindowEvent(const SDL_WindowEvent &wevent) {
    switch (wevent.event) {
        case SDL_WINDOWEVENT_RESIZED:
            width = wevent.data1;
            height = wevent.data2;
            break;
    }
}
