#pragma once

#include "types.h"

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

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
