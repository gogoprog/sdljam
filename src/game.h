#pragma once

#include "types.h"

struct Wave {
    int units;
    int speed;
    int hp;
};

struct Stats {
    int lifes;
    int money;
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
    void reset();
    void changeState(const State state);
    void nextWave();

    Stats stats;
    Wave currentWave;
    int waveCount{0};

  private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};
