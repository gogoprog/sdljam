#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "ecs.h"
#include "inputs.h"
#include "level.h"
#include "renderer.h"
#include "context.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

void loadData(Renderer &renderer);

int main(int arc, char **argv) {
    bool quit{false};

    SDL_Init(SDL_INIT_VIDEO);

    Context & context = Context::get();
    auto & renderer = context.renderer;
    auto & inputs = context.inputs;
    auto & engine = context.engine;
    auto & level = context.level;

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

    {

        class TestSystem : public System {
          public:
            TestSystem() {
                componentsNames.push_back("Dummy");
            }

            void onEntityAdded(Entity &entity) override {
                puts("entity addeD");
            }

            void updateSingle(Entity &entity) override {
                puts("yep");
            }
        };

        struct Dummy : public Component {
            Dummy() : Component("Dummy"){};
        };

        engine.addSystem(new TestSystem());

        auto e = std::make_shared<Entity>();

        e->add<Dummy>();
        engine.addEntity(e);



        engine.update();
    }

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

        level.render(renderer);

        viewer("Turret");

        engine.update();
        renderer.update();

        if (inputs.isKeyJustPressed(SDL_SCANCODE_ESCAPE)) {
            quit = true;
        }
    }

    return 0;
}
