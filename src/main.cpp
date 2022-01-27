#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "inputs.h"
#include "renderer.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int arc, char **argv) {
    bool quit{false};

    SDL_Init(SDL_INIT_VIDEO);

    Renderer renderer;
    Inputs inputs;

    renderer.init();

    renderer.loadAtlas("Turret");

    int f = 0;

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

        if (inputs.isJustPressed(SDL_SCANCODE_SPACE)) {
            f++;
            std::cout << f << std::endl;
        }
        renderer.draw({32, 32}, "Turret", f);

        renderer.update();

        if (inputs.isJustPressed(SDL_SCANCODE_ESCAPE)) {
            quit = true;
        }
    }

    return 0;
}
