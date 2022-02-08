#include "audio.h"

#include "types.h"
#include <SDL.h>
#include <SDL_mixer.h>
#include <iostream>

struct Sound {
    Mix_Chunk *chunk;
};

struct Audio::Pimpl {
    Map<String, Sound> sounds;
};

Audio::Audio() : pimpl(new Audio::Pimpl()) {
}

Audio::~Audio() = default;

void Audio::init() {
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);
}

void Audio::loadSound(const String &name) {
    std::string path;
    path = "res/" + name + ".wav";

    Sound sound;

    sound.chunk = Mix_LoadWAV(path.c_str());

    pimpl->sounds[name] = sound;

    std::cout << "Loaded sound '" << name << "'" << std::endl;
}

void Audio::playSound(const String &name) {
    auto &sound = pimpl->sounds[name];

    auto ret = Mix_PlayChannel(-1, sound.chunk, 0);

    if (ret == -1) {
        Mix_PlayChannel(rand() % 8, sound.chunk, 0);
    }
}
