#pragma once

#include "types.h"

struct Wave {
    int units;
};

class Game {
  public:
    enum class State {
        INITIATING,
        BUILDING_ROADS,
        BUILDING_TURRETS,
        PLAYING,
        WINNING,
        LOSING,
    };

    Game();
    ~Game();

    void init();
    void changeState(const State state);

    void nextWave();

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
