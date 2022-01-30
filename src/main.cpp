#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "context.h"
#include "game.h"

void loadData(Renderer &renderer);

int main(int arc, char **argv) {
    bool quit{false};

    SDL_Init(SDL_INIT_VIDEO);

    Context &context = Context::get();
    auto &renderer = context.renderer;
    auto &inputs = context.inputs;
    auto &engine = context.engine;
    auto &level = context.level;
    auto &game = context.game;

    renderer.init();

    loadData(renderer);

    auto viewer = [&](const char *atlasname) {
        static int frame = 0;
        frame %= renderer.getFramesCount(atlasname);
        renderer.draw({0, 0}, atlasname, frame, false);
        renderer.draw({256, 256}, atlasname, frame);

        if (inputs.isKeyJustPressed(SDL_SCANCODE_LEFT)) {
            frame--;
            if (frame < 0)
                frame = renderer.getFramesCount(atlasname) - 1;
        }
        if (inputs.isKeyJustPressed(SDL_SCANCODE_RIGHT)) {
            frame++;
        }
        if (inputs.isMouseJustPressed(1)) {
            auto pos = inputs.getMousePosition();
            renderer.setPivot(atlasname, frame, pos);
        }
        if (inputs.isKeyJustPressed(SDL_SCANCODE_SPACE)) {
            renderer.exportAtlas(atlasname);
        }
    };

    game.init();

    auto last_ticks = SDL_GetTicks();
    while (!quit) {
        SDL_Event event;
        auto current_ticks = SDL_GetTicks();
        auto delta_time = (current_ticks - last_ticks) / 1000.0f;
        last_ticks = current_ticks;

        inputs.update();
        renderer.clear();

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_MOUSEMOTION:
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    inputs.handle(event);
                    break;
                case SDL_QUIT:
                    quit = true;
                    break;
                case SDL_WINDOWEVENT:
                    renderer.onWindowEvent(event.window);
                    break;
            }
        }

        level.render(renderer);
        engine.update(delta_time);

        /* viewer("Bullets"); */

        renderer.update();

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            quit = true;
        }
    }

    return 0;
}
