#include "ImageFileReader.h"
#include <cmath>
void ImageFileReader::allocateRowPointers() {
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        if (!row_pointers) {
            std::cout << "Error ImageRead: Can't allocate memory";
            exit(41);
        }
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
    }

void ImageFileReader::checkFileHeader(FILE* file) {
    png_byte header[8];
    fread(header, sizeof(png_byte), 8, file);
    if (png_sig_cmp(header, 0, 8)){
        std::cout << "Error: File isn't have png format" << std::endl;
        fclose(file);
        exit(41);
    }
}

void ImageFileReader::getPngParam() {
    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);
    number_of_passes = png_set_interlace_handling(png_ptr);
}

void ImageFileReader::transforImage() {
    if (color_type == PNG_COLOR_TYPE_RGB || 
    color_type == PNG_COLOR_TYPE_GRAY || 
    color_type == PNG_COLOR_TYPE_PALETTE) {
        png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
        png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
    }
}

int ImageFileReader::adjustValue(int value) {
    if (value < 0) return 0;
    if (value > 255) return 255;
    return value;
}
    
ImageFileReader::ImageFileReader() {
    width = 0;
    height = 0;
    color_type = 0;
    bit_depth = 0;
    number_of_passes = 0;
    png_ptr = nullptr;
    info_ptr = nullptr;
    row_pointers = nullptr;
}

ImageFileReader::~ImageFileReader() {
    if (row_pointers != nullptr) {
        for (int y = 0; y < height; y++) {
            free(row_pointers[y]);
        }
        free(row_pointers);
    }
    if (png_ptr != nullptr) {
        png_destroy_read_struct(&png_ptr, &info_ptr, nullptr);
    }
}

void ImageFileReader::resolve(const char* filename) {
    FILE* input_png = fopen(filename, "rb");
    if (!input_png){
        std::cout << "Cannot read file: " << filename << std::endl;
        exit(41);
    }
    checkFileHeader(input_png);

    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        std::cout << "Error in png structure" << std::endl;
        fclose(input_png);
        exit(41);
    }

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
        std::cout << "Error in png info-structure" << std::endl;
        png_destroy_read_struct(&png_ptr, (png_infopp)nullptr, (png_infopp)nullptr);
        fclose(input_png);
        exit(41);
    }

    if (setjmp(png_jmpbuf(png_ptr))){
        png_destroy_read_struct(&png_ptr, &info_ptr, (png_infopp)nullptr);
        fclose(input_png);
        exit(41);
    }
    png_init_io(png_ptr, input_png);
    png_set_sig_bytes(png_ptr, 8);
    png_read_info(png_ptr, info_ptr);
    getPngParam();
    transforImage();
    png_read_update_info(png_ptr, info_ptr);
    getPngParam();
    number_of_passes = png_set_interlace_handling(png_ptr);
    allocateRowPointers();
    png_read_image(png_ptr, row_pointers);
    fclose(input_png);
}

int ImageFileReader::getWidth() {
    return width;
}

int ImageFileReader::getHeight() {
    return height;
}

int ImageFileReader::getColortype() {
    return color_type;
}

int ImageFileReader::getBitDepth() {
    return bit_depth;
}

void ImageFileReader::write(const char* filename) {
    FILE *output_png = fopen(filename, "wb");
    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (!png_ptr) {
        std::cout << "Error in write png structure" << std::endl;
        fclose(output_png);
        exit(41);
    }

    png_infop info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr) {
        std::cout << "Error in write info-png structure" << std::endl;
        fclose(output_png);
        exit(41);
    }
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(output_png);
        exit(41);
    }

    png_init_io(png_ptr, output_png);
    if (setjmp(png_jmpbuf(png_ptr))) {
        std::cout << "Error to write a header" << std::endl;
        fclose(output_png);
        exit(41);
    }

    png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, 
                PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png_ptr, info_ptr);

    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(output_png);
        exit(41);
    }
        
    png_write_image(png_ptr, row_pointers);
    if (setjmp(png_jmpbuf(png_ptr))) {
        fclose(output_png);
        exit(41);
    }

    png_write_end(png_ptr, nullptr);
    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(output_png);
}

void ImageFileReader::savePixel(int x, int y, Rgbacolour colour) {
    if (!(x >= 0 && x < width && y >= 0 && y < height)) return;
    png_bytep pixel = &(row_pointers[y][x * 4]);
    pixel[0] = colour.red;
    pixel[1] = colour.green;
    pixel[2] = colour.blue;
    pixel[3] = colour.alpha;
}

Rgbacolour ImageFileReader::getPixel(int x, int y) {
    Rgbacolour colourPixel;
    if (!((x >= 0 && x < width && y >= 0 && y < height))) {
        return colourPixel;
    }
    colourPixel.red = row_pointers[y][x * 4 + 0];
    colourPixel.green = row_pointers[y][x * 4 + 1];
    colourPixel.blue = row_pointers[y][x * 4 + 2];
    colourPixel.alpha = row_pointers[y][x *4  + 3];
    return colourPixel;
}

void ImageFileReader::drawCircleBorder(int x0, int y0, int radius, Rgbacolour color) {
    int x = 0;
    int y = radius;
    int delta = 3 - 2 * y;
    while (x <= y) {
        savePixel(x0 + x, y0 + y, color);
        savePixel(x0 + x, y0 - y, color);
        savePixel(x0 - x, y0 + y, color);
        savePixel(x0- x, y0 - y, color);
        savePixel(x0 + y, y0 + x, color);
        savePixel(x0 + y, y0 - x, color);
        savePixel(x0 - y, y0 + x, color);
        savePixel(x0 - y, y0 - x, color);
        delta += delta < 0 ? 4 * x + 6 : 4 * (x - y--) + 10;
        ++x;
    }
}

void ImageFileReader::drawCircleArea(int x0, int y0, int radius, Rgbacolour color) {
    for (int y = std::max(0, y0 - radius); y <= std::min(height - 1, y0 + radius); y++) {
        for (int x = std::max(0, x0 - radius); x <= std::min(width - 1, x0 + radius); x++) {
            if ((x - x0) * (x - x0)  + (y - y0) * (y - y0)  <= radius * radius) {
                savePixel(x, y, color);
            }
        }
    }
}

void ImageFileReader::drawLine(int beginX, int beginY, int endX, int endY, int thickness, Rgbacolour color) {
    int absDeltaX = abs(endX - beginX);
    int absDeltaY = abs(endY - beginY);
    int signedDeltaX = beginX < endX ? 1: -1;
    int signedDeltaY = beginY < endY ? 1: -1;
    int err = absDeltaX - absDeltaY;
    int currentX = beginX;
    int currentY = beginY;
    while (currentX != endX || currentY != endY) {
        drawCircleArea(currentX, currentY, thickness / 2, color);
        drawCircleBorder(currentX, currentY, thickness / 2, color);
        int doubleError = 2 * err;
        if (doubleError > -absDeltaY) {
            err -= absDeltaY;
            currentX += signedDeltaX;
        }
        if (doubleError < absDeltaX) {
            err += absDeltaX;
            currentY += signedDeltaY;
        }
    }
    drawCircleBorder(endX, endY, thickness / 2, color);
    drawCircleArea(endX, endY, thickness /2, color);
}

void ImageFileReader::swapCoordinates(int& leftX, int& leftY, int& rightX, int& rightY) {
    int max_x = std::max(leftX, rightX);
    int min_x = std::min(leftX, rightX);
    int max_y = std::max(leftY, rightY);
    int min_y = std::min(leftY, rightY);
    leftX = min_x;
    leftY = min_y;
    rightX = max_x;
    rightY = max_y;
}

void ImageFileReader::drawBorderRect(int leftX, int leftY, int rightX, int rightY, Rgbacolour color, int thickness, bool fill, Rgbacolour fill_color){
    swapCoordinates(leftX,leftY,rightX,rightY);
    if (fill) drawAreaRect(leftX, leftY, rightX, rightY, fill_color);
    drawLine(leftX, leftY, rightX, leftY, thickness, color);
    drawLine(leftX, leftY, leftX, rightY, thickness, color);
    drawLine(leftX, rightY, rightX, rightY, thickness, color);
    drawLine(rightX, leftY, rightX, rightY, thickness, color);
}
    
void ImageFileReader::drawAreaRect(int leftX, int leftY, int rightX, int rightY, Rgbacolour fill_color) {
    swapCoordinates(leftX,leftY,rightX,rightY);
    for (int y = leftY + 1; y < rightY;y++){
        for (int x = leftX + 1; x < rightX;x++){
            savePixel(x, y, fill_color);
        }
    }
}
    
std::vector<std::pair<int, int>> ImageFileReader::getCorners(int x0, int y0, int radius) {
    std::vector<std::pair<int, int>> corners;
    for (int i = 0; i < 6;i++){
        corners.push_back({x0 + radius * std::cos(i * (M_PI * 2 / 6)), y0 + radius * std::sin(i * (M_PI * 2 / 6))});
    }
    return corners;
}

void ImageFileReader::drawHexagon(int x0, int y0, int radius, Rgbacolour color, int thickness, bool fill, Rgbacolour fill_color) {
    std::vector<std::pair<int, int>> corners = getCorners(x0, y0, radius);
    if (fill) {
        for (int y = std::max(0, y0 - radius); y <= std::min(height - 1, y0 + radius);y++) {
            for (int x = std::max(0, x0 - radius); x <= std::min(width - 1, x0 + radius);x++) {
                if (coordinateInHexagon(x, y, corners)) {
                    savePixel(x, y, fill_color);
                }
            }
        }
    }
    for (int i = 0; i < 6; i++) {
        drawLine(corners[i].first, corners[i].second, corners[(i + 1) % 6].first, corners[(i + 1) % 6].second, thickness, color);
    }
}

int ImageFileReader::coordinateInHexagon(int x, int y, const std::vector<std::pair<int, int>>& corners) {
    int intersectionCount = 0;
    for (int i = 0; i < 6; i++) {
        int next = (i + 1) % 6;
        if ((corners[i].second > y && corners[next].second <= y) || (corners[next].second > y && corners[i].second <= y)) {
            if ((corners[next].second - corners[i].second) < 0) {
                if ((x * (corners[next].second - corners[i].second)) < ((corners[next].second - corners[i].second) * corners[i].first + (y - corners[i].second) * (corners[next].first - corners[i].first))) {
                    intersectionCount++;
                }
            } 
            else {
                if ((x * (corners[next].second - corners[i].second)) > ((corners[next].second - corners[i].second) * corners[i].first + (y - corners[i].second) * (corners[next].first - corners[i].first))) {
                    intersectionCount++;
                }
            }
        }
    }
    return (intersectionCount % 2);
}

void ImageFileReader::copyArea(int leftX, int leftY, int rightX, int rightY, int destX, int destY) {
    swapCoordinates(leftX, leftY, rightX, rightY);
    int copyDimensionX = rightX - leftX + 1;
    int copyDimensionY = rightY - leftY + 1;
    int maxCopyWidth = std::min(width, copyDimensionX);
    int maxCopyHeight = std::min(height, copyDimensionY);
    std::vector<std::vector<Rgbacolour>> colors(maxCopyHeight, std::vector<Rgbacolour>(maxCopyWidth, Rgbacolour(0, 0, 0)));
    for (int y = 0; y < maxCopyHeight; y++) {
        for (int x = 0; x < maxCopyWidth; x++) {
            int sourceX = x + leftX;
            int sourceY = y + leftY;
            if (sourceX >= 0 && sourceX < width && sourceY >= 0 && sourceY < height) {
                colors[y][x] = getPixel(sourceX, sourceY);
            }
        }
    }
    for (int y = 0; y < maxCopyHeight; y++) {
        for (int x = 0; x < maxCopyWidth; x++) {
            int destinationX = x + destX;
            int destinationY = y + destY;
            if (destinationX >= 0 && destinationX < width && destinationY >= 0 && destinationY < height) {
                savePixel(destinationX, destinationY, colors[y][x]);
            }
        }
    }
    colors.clear();
}

void ImageFileReader::bitwiseImage(std::string op, int threshold_red, int threshold_green, int threshold_blue){
    for (int y = 0;y < height;y++){
        for (int x =0;x < width;x++){
            Rgbacolour color = getPixel(x,y);
            int result;
            if (op == "and"){
                result = color.red & color.blue & color.green;
            }
            if (op == "xor"){
                result = color.red ^ color.blue ^ color.green;
            }
            Rgbacolour newColor(0,0,0);
            if (result > threshold_red){
                newColor.red = std::min(255, result);
                }
            if (result > threshold_blue){
                newColor.blue = std::min(255, result);
                }
            if (result > threshold_green){
                newColor.green = std::min(255, result);
            }
            savePixel(x, y, newColor);
        }
    }
}