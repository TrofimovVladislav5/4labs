#include <iostream>
#include <vector>
#include <png.h>
#include <cmath> // Для использования функций cos() и sin()

#define RED 0
#define GREEN 1
#define BLUE 2
#define ALPHA 3

class ImagePNG {
private:
    int width;
    int height;
    png_byte color_type;
    png_byte bit_depth;
    int number_of_passes;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;

    void allocateRowPointers(){
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
    }

public:
    ImagePNG() {
        width = 0;
        height = 0;
        color_type = 0;
        bit_depth = 0;
        png_ptr = nullptr;
        info_ptr = nullptr;
        row_pointers = nullptr;
    }

    ~ImagePNG() {
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

    void getPngParam(){
        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        number_of_passes = png_set_interlace_handling(png_ptr);
    }

    void checkFileHeader(FILE* file){
        png_byte header[8];
        fread(header, sizeof(png_byte), 8, file);
        if (png_sig_cmp(header, 0, 8)){
            std:: cout << "Error: File isn't have png format" << std::endl;
            return;
        }
    }

    void readPNGImage(const char* filename) {
        FILE* input_png = fopen(filename, "rb");
        checkFileHeader(input_png);
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        info_ptr = png_create_info_struct(png_ptr);
        png_init_io(png_ptr, input_png);
        png_set_sig_bytes(png_ptr, 8);
        png_read_info(png_ptr, info_ptr);
        getPngParam();
        number_of_passes = png_set_interlace_handling(png_ptr);
        allocateRowPointers();
        png_read_image(png_ptr, row_pointers);
        fclose(input_png);
    }

    void writePNGImage(const char* filename) {
        FILE* output_png = fopen(filename, "wb");
        png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        png_infop info_ptr = png_create_info_struct(png_ptr);
        png_init_io(png_ptr, output_png);
        png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
        png_write_info(png_ptr, info_ptr);
        png_write_image(png_ptr, row_pointers);
        png_write_end(png_ptr, nullptr);
        png_destroy_write_struct(&png_ptr, &info_ptr);
        fclose(output_png);
    }

    void writePNGPixel(int x, int y, png_byte red, png_byte green, png_byte blue, png_byte alpha = 255) {
    if (x >= 0 && x < width && y >= 0 && y < height) {
        png_bytep pixel;
        if (color_type == PNG_COLOR_TYPE_RGB) {
            pixel = &(row_pointers[y][x * 3]);
            pixel[RED] = red;
            pixel[GREEN] = green;
            pixel[BLUE] = blue;
        } else if (color_type == PNG_COLOR_TYPE_RGBA) {
            pixel = &(row_pointers[y][x * 4]);
            pixel[RED] = red;
            pixel[GREEN] = green;
            pixel[BLUE] = blue;
            pixel[ALPHA] = alpha;
        }
    }
}


    void drawRectangle(int left, int up, int right, int down, png_byte red, png_byte green, png_byte blue, png_byte alpha = 255) {
        if (row_pointers != nullptr) {
            for (int y = down; y < up; y++) {
                for (int x = left; x < right; x++) {
                    writePNGPixel(x, y, red, green, blue, alpha);
                }
            }
        }
    }

    void setRectangle(int left, int up, int right, int down){

    }


//     // void floodFill(int x, int y, png_byte red, png_byte green, png_byte blue, png_byte alpha = 255) {
//     //     if (x < 0 || x >= width || y < 0 || y >= height) {
//     //         return; 
//     //     }
//     //     png_bytep pixel = &(row_pointers[y][x * 4]);
//     //     if (pixel[RED] == red && pixel[GREEN] == green && pixel[BLUE] == blue) {
//     //         return;
//     //     }
//     //     writePNGPixel(x, y, red, green, blue, alpha); 
//     //     floodFill(x + 1, y, red, green, blue, alpha);
//     //     floodFill(x - 1, y, red, green, blue, alpha);
//     //     floodFill(x, y + 1, red, green, blue, alpha);
//     //     floodFill(x, y - 1, red, green, blue, alpha);
//     // }

    void setLineBresenham(int x0, int y0, int x1, int y1, std::vector<std::pair<int, int>>& linePoints) {
        int deltaX = x1 - x0;
        int deltaY = y1 - y0;
        int absDeltaX = std::abs(deltaX);
        int absDeltaY = std::abs(deltaY);
        int accretion = 0;
        int direction = 0;
        if (absDeltaX >= absDeltaY) {
            int y = y0;
            if (deltaY != 0) {
                direction = (deltaY > 0) ? 1 : -1;
            }
            if (deltaX > 0) {
                for (int x = x0; x <= x1; x++) {
                    writePNGPixel(x, y, 255, 0, 0);
                    linePoints.push_back(std::make_pair(x, y));
                    accretion += absDeltaY;
                    if (accretion >= absDeltaX) {
                        accretion -= absDeltaX;
                        y += direction;
                    }
                }
            } else {
                for (int x = x0; x >= x1; x--) {
                    writePNGPixel(x,y, 255, 0, 0);
                    linePoints.push_back(std::make_pair(x, y));
                    accretion += absDeltaY;
                    if (accretion >= absDeltaX) {
                        accretion -= absDeltaX;
                        y += direction;
                    }
                }
            }
        } else {
            int x = x0;
            if (deltaX != 0) {
                direction = (deltaX > 0) ? 1 : -1;
            }
            if (deltaY > 0) {
                for (int y = y0; y <= y1; y++) {
                    writePNGPixel(x,y, 255, 0, 0);
                    linePoints.push_back(std::make_pair(x, y));
                    accretion += absDeltaX;
                    if (accretion >= absDeltaY) {
                        accretion -= absDeltaY;
                        x += direction;
                    }
                }
            } else {
                for (int y = y0; y >= y1; y--) {
                    writePNGPixel(x,y, 255, 0, 0);
                    linePoints.push_back(std::make_pair(x, y));
                    accretion += absDeltaX;
                    if (accretion >= absDeltaY) {
                        accretion -= absDeltaY;
                        x += direction;
                    }
                }
            }
        }
    }

    void setHexagon(int centerX, int centerY, int radius, int fill_colour){
        int* arrX = new int[7];
        int* arrY = new int[7];
        for (int i = 0;i < 6;i++){
            arrX[i] = centerX + radius * std::cos(i * (M_PI / 3)); // Используем M_PI напрямую
            arrY[i] = centerY + radius * std::sin(i * (M_PI / 3)); // Используем M_PI напрямую
        }
        arrX[6] = arrX[0];
        arrY[6] = arrY[0];
        std::vector<std::pair<int , int>> linePoints;
        if (fill_colour){
            for (int i = 0; i < 6;i++) {
                setLineBresenham(arrX[i],arrY[i], arrX[i + 1], arrY[i + 1], linePoints);
            }
            int minY = arrY[0];
            int maxY = arrY[0];
            for (int i = 1; i < 7; i++) {
                minY = std::min(minY, arrY[i]);
                maxY = std::max(maxY, arrY[i]);
            }

            for (int y = minY; y < maxY; y++){
                int minX = INT_MAX;
                int maxX = INT_MIN;
                for (auto& point: linePoints){
                    if (point.second == y){
                        minX = std::min(minX, point.first);
                        maxX = std::max(maxX, point.first);
                    }
                }
                for (int x = minX + 1; x < maxX;x++){
                     writePNGPixel(x, y, 255, 0, 0);
                }
                 
            }
        }
        if (!fill_colour){
            for (int i = 0; i < 6;i++) {
                setLineBresenham(arrX[i],arrY[i], arrX[i + 1], arrY[i + 1], linePoints);
            }
        }
        
        linePoints.clear();
        delete[] arrX;
        delete[] arrY;
    }
};

// при рисовании шестиугольника с заливкой передаются координаты линии и от начала линии до начала конечной линии заполняются пикселями
// возможно нужно проходиться от начала линии не включая пиксели

int main() {
    const char* input_filename = "cubix.png";
    const char* output_filename = "output.png";

    ImagePNG image;
    image.readPNGImage(input_filename);
    // int x1 = 100;
    // int y1 = 300;
    // int x2 = 250;
    // int y2 = 100;
    // image.drawRectangle(x1, y1, x2, y2, 0, 0, 255,125);
    // image.setLineBresenham(x1,y1,x2,y2);
    image.setHexagon(100, 100, 50, 1);
    image.writePNGImage(output_filename);
    std::cout << "Создана копия изображения в файле " << output_filename << std::endl;

    return 0; 
}