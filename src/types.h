#pragma once

#include <cmath>
#include <map>
#include <memory>
#include <numbers>
#include <string>
#include <vector>

using String = std::string;
template <typename T> using Vector = std::vector<T>;
template <typename T> using UniquePtr = std::unique_ptr<T>;
template <typename T> using SharedPtr = std::shared_ptr<T>;
template <typename K, typename V> using Map = std::map<K, V>;

class Vector2 {
  public:
    Vector2() = default;
    Vector2(int _x, int _y) : x(_x), y(_y) {
    }
    Vector2(float _x, float _y) : x(_x), y(_y) {
    }
    Vector2(double _x, double _y) : x(_x), y(_y) {
    }
    float x;
    float y;

    float getLength() const {
        return std::sqrt(x * x + y * y);
    }
};

inline Vector2 operator+(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x + v2.x, v1.y + v2.y};
}

inline Vector2 operator-(const Vector2 &v1, const Vector2 &v2) {
    return {v1.x - v2.x, v1.y - v2.y};
}

inline Vector2 operator*(const Vector2 &v1, const float v) {
    return {v1.x * v, v1.y * v};
}

inline bool operator<(const Vector2 &v1, const Vector2 &v2) {
    if (v1.y == v2.y)
        return v1.x < v2.x;
    return v1.y < v2.y;
}

inline float clampAngle(const float input) {
    float result = input;

    while (result < 0) {
        result += 360;
    }
    while (result > 360) {
        result -= 360;
    }

    return result;
}
