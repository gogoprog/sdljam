#include "audio.h"

#include "types.h"
#include <SDL3/SDL.h>
#include <SDL3_mixer/SDL_mixer.h>
#include <iostream>

struct Sound {
    MIX_Audio *audio;
};

struct Audio::Pimpl {
    MIX_Mixer *mixer = nullptr;
    Map<String, Sound> sounds;
};

Audio::Audio() : pimpl(new Audio::Pimpl()) {
}

Audio::~Audio() {
    for (auto &kv : pimpl->sounds) {
        MIX_DestroyAudio(kv.second.audio);
    }
    if (pimpl->mixer != nullptr) {
        MIX_DestroyMixer(pimpl->mixer);
    }
    MIX_Quit();
}

void Audio::init() {
    MIX_Init();
    pimpl->mixer = MIX_CreateMixerDevice(SDL_AUDIO_DEVICE_DEFAULT_PLAYBACK, NULL);
    if (pimpl->mixer != nullptr) {
        MIX_SetMixerGain(pimpl->mixer, 1.0f);
    }
}

void Audio::loadSound(const String &name) {
    std::string path;
    path = "res/" + name + ".wav";

    Sound sound;
    sound.audio = MIX_LoadAudio(pimpl->mixer, path.c_str(), true);

    pimpl->sounds[name] = sound;

    std::cout << "Loaded sound '" << name << "'" << std::endl;
}

void Audio::playSound(const String &name) {
    auto &sound = pimpl->sounds[name];
    MIX_PlayAudio(pimpl->mixer, sound.audio);
}
