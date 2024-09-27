#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <png.h>
#include <string>
struct Flag {
    bool inputed;
    std::string value;
};

struct Rgbacolour {
    int red;
    int green;
    int blue;
    int alpha;

    Rgbacolour(png_byte r, png_byte g, png_byte b, png_byte a = 255);
    Rgbacolour();
};

#endif