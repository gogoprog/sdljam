#pragma once

#include <string>
#include <vector>
#include <memory>

using String = std::string;
template <typename T> using Vector = std::vector<T>;
template <typename T> using UniquePtr = std::unique_ptr<T>;
template <typename T> using SharedPtr = std::shared_ptr<T>;

class Vector2 {
  public:
    Vector2() = default;
    Vector2(int _x, int _y) : x(_x), y(_y) {
    }
    float x;
    float y;
};
