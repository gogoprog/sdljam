#pragma once

#include "types.h"

class Game {
  public:
    enum class State {
        INITIATING,
        PREPARING,
        PLAYING,
        WINNING,
        LOSING,
    };

    enum class Mode {
        FIRING,
        TURRET_BUILDING,
        ROAD_BUILDING,
    };

    Game();
    ~Game();

    void init();
    void changeMode(const Mode mode);

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
