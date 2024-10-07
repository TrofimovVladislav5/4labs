#pragma once
#include <utility>
#include <cstddef>

enum class Direction{
    horizontal,
    vertical
};

struct hashFunc {
    size_t operator()(std::pair<int, int> coordinate) const;
};