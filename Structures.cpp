#include "Structures.h"
#include <cstddef>

size_t hashFunc::operator()(std::pair<int, int> coordinate) const { return coordinate.first + coordinate.second;}