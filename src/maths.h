#pragma once

class Vector2 {
  public:
    Vector2() = default;
    Vector2(int _x, int _y) : x(_x), y(_y) {
    }
    float x;
    float y;
};
