#pragma once

#include "types.h"

class Audio {
  public:
    Audio();
    ~Audio();

    void init();
    void clear();
    void update();

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
