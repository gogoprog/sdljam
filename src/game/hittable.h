#pragma once

#include "../context.h"

struct Hittable : public Component {
    inline static String name = "Hittable";

    float radius = 32;
};
