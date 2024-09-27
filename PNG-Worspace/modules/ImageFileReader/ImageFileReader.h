#pragma once
#include <iostream>
#include <map>
#include <vector>
#include "Structures.h"

class ImageFileReader {
private:
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    int number_of_passes;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;

    void allocateRowPointers();
    void checkFileHeader(FILE* file) ;
    void getPngParam();
    void transforImage();
    int adjustValue(int value);
    
public:
    ImageFileReader();
    ~ImageFileReader();
    void resolve(const char* filename);
    int getWidth();
    int getHeight();
    int getColortype();
    int getBitDepth();
    void write(const char* filename) ;
    void savePixel(int x, int y, Rgbacolour colour);
    Rgbacolour getPixel(int x, int y);
    void drawCircleBorder(int x0, int y0, int radius, Rgbacolour color);
    void drawCircleArea(int x0, int y0, int radius, Rgbacolour color);
    void drawLine(int beginX, int beginY, int endX, int endY, int thickness, Rgbacolour color);
    void swapCoordinates(int& leftX, int& leftY, int& rightX, int& rightY);
    void drawBorderRect(int leftX, int leftY, int rightX, int rightY, Rgbacolour color, int thickness, bool fill, Rgbacolour fill_color);
    void drawAreaRect(int leftX, int leftY, int rightX, int rightY, Rgbacolour fill_color);
    std::vector<std::pair<int, int>> getCorners(int x0, int y0, int radius);
    void drawHexagon(int x0, int y0, int radius, Rgbacolour color, int thickness, bool fill, Rgbacolour fill_color);
    int coordinateInHexagon(int x, int y, const std::vector<std::pair<int, int>>& corners);
    void copyArea(int leftX, int leftY, int rightX, int rightY, int destX, int destY);
    void bitwiseImage(std::string op, int threshold_red, int threshold_green, int threshold_blue);
};