#include "Structures.h"
#include <png.h>
Rgbacolour::Rgbacolour() : red(0), green(0), blue(0), alpha(0) {}

Rgbacolour::Rgbacolour(png_byte r, png_byte g, png_byte b, png_byte a)
    : red(r), green(g), blue(b), alpha(a) {}