#pragma once

#include "types.h"

class Audio {
  public:
    Audio();
    ~Audio();

    void init();
    void update();

    void loadSound(const String &name);
    void playSound(const String &name);

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
