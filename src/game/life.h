#pragma once

#include "../context.h"

struct Life : public Component {
    inline static String name = "Life";

    int hp{100};
};
