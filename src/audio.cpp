#include "audio.h"

#include "types.h"

struct Audio::Pimpl {

};

Audio::Audio() : pimpl(new Audio::Pimpl()) {
}

Audio::~Audio() = default;

void Audio::init() {
}
