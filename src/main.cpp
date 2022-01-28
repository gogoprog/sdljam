#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "inputs.h"
#include "renderer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void loadData(Renderer & renderer);

int main(int arc, char **argv) {
    bool quit{false};

    SDL_Init(SDL_INIT_VIDEO);

    Renderer renderer;
    Inputs inputs;

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

    while (!quit) {
        SDL_Event event;

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
            }
        }

        viewer("Turret");

        renderer.update();

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            quit = true;
        }
    }

    return 0;
}
