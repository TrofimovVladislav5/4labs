#include <iostream>
#include <cmath>
#include <png.h>
#include <vector>
#include <map>
#include <set>
#include <getopt.h>
#include <regex>

#define HELP_INDEX 'h'
#define OUTPUT_INDEX 'o'
#define INPUT_INDEX 'i'
#define INFO_INDEX 1000
#define RECT_INDEX 1001
#define LEFT_UP_INDEX 1002
#define RIGHT_DOWN_INDEX 1003
#define THICKNESS_INDEX 1004
#define COLOR_INDEX 1005
#define FILL_INDEX 1006
#define FILL_COLOR_INDEX 1007
#define HEXAGON_INDEX 1008
#define CENTER_INDEX 1009
#define RADIUS_INDEX 1010
#define DEST_LEFT_UP_INDEX 1011
#define COPY_INDEX 1012

struct Flag {
    bool inputed;
    std::string value;
};

struct Rgbacolour {
    int red;
    int green;
    int blue;
    int alpha;

    Rgbacolour(png_byte r, png_byte g, png_byte b, png_byte a = 255) {
        red = r;
        green = g;
        blue = b;
        alpha = a;
    }
};

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

    void allocateRowPointers() {
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        if (!row_pointers) {
            std::cout << "Error ImageRead: Can't allocate memory";
            exit(41);
        }
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
    }

    static void checkFileHeader(FILE* file) {
        png_byte header[8];
        fread(header, sizeof(png_byte), 8, file);
        if (png_sig_cmp(header, 0, 8)){
            std::cout << "Error: File isn't have png format" << std::endl;
            fclose(file);
            exit(41);
        }
    }

    void getPngParam() {
        width = png_get_image_width(png_ptr, info_ptr);
        height = png_get_image_height(png_ptr, info_ptr);
        bit_depth = png_get_bit_depth(png_ptr, info_ptr);
        color_type = png_get_color_type(png_ptr, info_ptr);
        number_of_passes = png_set_interlace_handling(png_ptr);
    }

    void transforImage() {
        if (color_type == PNG_COLOR_TYPE_RGB || 
        color_type == PNG_COLOR_TYPE_GRAY || 
        color_type == PNG_COLOR_TYPE_PALETTE) {
            png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
            png_set_add_alpha(png_ptr, 0xFF, PNG_FILLER_AFTER);
        }
    }
public:
    ImageFileReader() {
        width = 0;
        height = 0;
        color_type = 0;
        bit_depth = 0;
        number_of_passes = 0;
        png_ptr = nullptr;
        info_ptr = nullptr;
        row_pointers = nullptr;
    }

    ~ImageFileReader() {
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

    void resolve(const char* filename) {
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

    int getWidth() {
        return width;
    }

    int getHeight() {
        return height;
    }

    int getColortype() {
        return color_type;
    }

    int getBitDepth() {
        return bit_depth;
    }

    void write(const char* filename) {
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

    void savePixel(std::pair<int, int> coordinates, Rgbacolour colour) {
        if (!(coordinates.first >= 0 && coordinates.first < width && coordinates.second >= 0 && coordinates.second < height)) return;
        png_bytep pixel = &(row_pointers[coordinates.second][coordinates.first * 4]);
        pixel[0] = colour.red;
        pixel[1] = colour.green;
        pixel[2] = colour.blue;
        pixel[3] = colour.alpha;
    }

    Rgbacolour getPixel(std::pair<int, int> coordinates) {
        Rgbacolour colourPixel(0,0,0);
        if (!((coordinates.first >= 0 && coordinates.first < width && coordinates.second >= 0 && coordinates.second < height))) {
            return colourPixel;
        }
        colourPixel.red = row_pointers[coordinates.second][coordinates.first * 4 + 0];
        colourPixel.green = row_pointers[coordinates.second][coordinates.first * 4 + 1];
        colourPixel.blue = row_pointers[coordinates.second][coordinates.first * 4 + 2];
        colourPixel.alpha = row_pointers[coordinates.second][coordinates.first *4  + 3];
        return colourPixel;
    }
};

class IDraw : public ImageFileReader {
    virtual std::vector<std::pair<int, int>> getBorderCoordinates() = 0;
    virtual std::vector<std::pair<int, int>> getBorderCoordinates(int thickness) = 0;
    virtual std::vector<std::pair<int, int>> getAreaCoordinates() = 0;
};

class Shape : public IDraw {
public:
    Shape() = default;
    virtual ~Shape(){};
    virtual std::vector<std::pair<int, int>> getBorderCoordinates() override;
    virtual std::vector<std::pair<int, int>> getBorderCoordinates(int) override;
    virtual std::vector<std::pair<int, int>> getAreaCoordinates() override;
};

class Circle : public Shape {
private:
    std::pair<int, int> center;
    int radius;
public:
    Circle(std::pair<int, int>, int);
    std::vector<std::pair<int, int>> getBorderCoordinates() override;
    std::vector<std::pair<int, int>> getAreaCoordinates() override;
};

class Line : public Shape {
private:
    std::pair<int, int> begin;
    std::pair<int, int> end;
public:
    Line(std::pair<int, int>, std::pair<int, int>);
    std::vector<std::pair<int, int>> getBorderCoordinates() override;
    std::vector<std::pair<int, int>> getAreaCoordinates() override;
};

class Rectangle : public Shape {
private:
    std::pair<int, int> upperLeft;
    std::pair<int, int> lowerRight;
public:
    Rectangle(std::pair<int, int>, std::pair<int, int>);
    std::vector<std::pair<int, int>> getBorderCoordinates() override;
    std::vector<std::pair<int, int>> getBorderCoordinates(int thickness) override;
    std::vector<std::pair<int, int>> getAreaCoordinates() override;
};

class Polygon : public Shape {
private:
    std::pair<int, int> center;
    int radius;
    int corners;
public:
    Polygon(std::pair<int, int>, int, int);
    std::vector<std::pair<int, int>> getCornersCoordinates();
    std::vector<std::pair<int, int>> getBorderCoordinates() override;
    std::vector<std::pair<int, int>> getAreaCoordinates() override;
};

class ICopy : public Shape {
private:
    std::pair<int, int> upperLeft;
    std::pair<int, int> lowerRight;
    std::pair<int, int> destUpperLeft;
public:
    ICopy(std::pair<int, int> upper, std::pair<int, int> lower, std::pair<int, int> dest){
        int max_x = std::max(upper.first, lower.first);
        int min_x = std::min(upper.first, lower.first);
        int max_y = std::max(upper.second, lower.second);
        int min_y = std::min(upper.second, lower.second);
        upperLeft = std::make_pair(min_x, min_y);
        lowerRight = std::make_pair(max_x, max_y);
        destUpperLeft = dest;
    }

    std::vector<std::pair<int, int>> getSourceCoordiantes() {
        std::vector<std::pair<int, int>> sourceCoordinates;
        for (int y = upperLeft.second; y <= lowerRight.second;y++){
            for (int x = upperLeft.first; x <= lowerRight.first;x++){
                sourceCoordinates.push_back({x,y});
            }
        }
        return sourceCoordinates;
    }

    std::vector<std::pair<int, int>> getDestinationCoordinates() {
        std::vector<std::pair<int, int>> destinationCoordinates;
        std::pair<int, int> copyDimension = std::make_pair(lowerRight.first - upperLeft.first, lowerRight.second - upperLeft.second);
        for (int y = destUpperLeft.second; y <= destUpperLeft.second +  copyDimension.second;y++){
            for (int x = destUpperLeft.first; x <= destUpperLeft.first + copyDimension.first;x++){
                destinationCoordinates.push_back({x, y});
            }
        }
        return destinationCoordinates;
    }
};

std::vector<std::pair<int, int>> Shape::getBorderCoordinates() {
    std::vector<std::pair<int, int>> temp;
    std::cout << "global shape" << std::endl;
    return temp;
}

std::vector<std::pair<int, int>> Shape::getBorderCoordinates(int thickness) {
    std::vector<std::pair<int, int>> initialCoordinates = getBorderCoordinates();
    if (thickness < 2)
        return initialCoordinates;
    std::vector<std::pair<int, int>> result;
    for (std::pair<int, int> coordinate : initialCoordinates) {
        int radius = thickness / 2;
        Shape *circle = new Circle(coordinate, radius);
        std::vector<std::pair<int, int>> circlePixels = circle->getBorderCoordinates();
        std::vector<std::pair<int, int>> circleAreaPixels = circle->getAreaCoordinates();
        circlePixels.insert(circlePixels.end(), circleAreaPixels.begin(), circleAreaPixels.end());
        result.insert(result.end(), circlePixels.begin(), circlePixels.end());

        circlePixels.clear();
        circleAreaPixels.clear();
        delete circle;
    }
    initialCoordinates.clear();
    return result;
}

std::vector<std::pair<int, int>> Shape::getAreaCoordinates() {
    std::vector<std::pair<int, int>> result;
    std::vector<std::pair<int, int>> borderCoordinates = getBorderCoordinates();
    std::pair<int, int> borders_y = std::make_pair(INT_MAX, INT_MIN); 
    for (std::pair<int, int> coordinate : borderCoordinates)
    {
        borders_y.first = std::min(borders_y.first, coordinate.second);
        borders_y.second = std::max(borders_y.second, coordinate.second);
    }
    for (int y = borders_y.first + 1; y < borders_y.second; y++)
    {
        std::pair<int, int> borders_x = std::make_pair(INT_MAX, INT_MIN);
        for (std::pair<int, int> coordinate : borderCoordinates)
        {
            if (coordinate.second == y)
            {
                borders_x.first = std::min(borders_x.first, coordinate.first);
                borders_x.second = std::max(borders_x.second, coordinate.first);
            }
            for (int x = borders_x.first + 1; x < borders_x.second; x++)
            {
                result.emplace_back(x, y);
            }
        }
    }
    return result;
}

Circle::Circle(std::pair<int, int> center, int radius){
    this->center = center;
    this->radius = radius;
}

std::vector<std::pair<int, int>> Circle::getBorderCoordinates() {
    std::vector<std::pair<int, int>> result;
    int x = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;
    while (y >= x)
    {
        result.emplace_back(center.first + x, center.second + y);
        result.emplace_back(center.first + x, center.second - y);
        result.emplace_back(center.first - x, center.second + y);
        result.emplace_back(center.first - x, center.second - y);
        result.emplace_back(center.first + y, center.second + x);
        result.emplace_back(center.first + y, center.second - x);
        result.emplace_back(center.first - y, center.second + x);
        result.emplace_back(center.first - y, center.second - x);
        error = 2 * (delta + y) - 1;
        if ((delta < 0) && (error <= 0))
        {
            delta += 2 * ++x + 1;
            continue;
        }
        if ((delta > 0) && (error > 0))
        {
            delta -= 2 * --y + 1;
            continue;
        }
        delta += 2 * (++x - --y);
    }
    return result;
}

std::vector<std::pair<int, int>> Circle::getAreaCoordinates() {
    std::vector<std::pair<int, int>> result;
    std::vector<std::pair<int, int>> borderCoordinates = getBorderCoordinates();
    for (int x = center.first - radius; x <= center.first + radius; ++x) {
        for (int y = center.second - radius; y <= center.second + radius; ++y) {
            if (std::pow(x - center.first, 2) + std::pow(y - center.second, 2) <= std::pow(radius, 2)) {
                result.emplace_back(x, y);
            }
        }
    }
    return result;
}

Line::Line(std::pair<int, int> begin, std::pair<int, int> end) {
        this->begin = begin;
        this->end = end;
}

std::vector<std::pair<int, int>> Line::getBorderCoordinates() {
    std::vector<std::pair<int, int>> linePoints;
    std::pair<int, int> absDelta = std::make_pair(abs(end.first - begin.first), abs(end.second - begin.second));
    std::pair<int, int> signedDelta = std::make_pair(begin.first < end.first ? 1 : -1, begin.second < end.second ? 1 : -1);
    int err = absDelta.first - absDelta.second;
    std::pair<int, int> current = std::make_pair(begin.first, begin.second);
    while (current.first != end.first || current.second != end.second) {
        linePoints.emplace_back(current.first, current.second);
        int doubleError = 2 * err;
        if (doubleError > -absDelta.second) {
            err -= absDelta.second;
            current.first += signedDelta.first;
        }
        if (doubleError < absDelta.first) {
            err += absDelta.first;
            current.second += signedDelta.second;
        }
    }
    return linePoints;
}

std::vector<std::pair<int, int>> Line::getAreaCoordinates() {
    return Line::getBorderCoordinates();
}

Rectangle::Rectangle(std::pair<int, int> first, std::pair<int, int> second) {
    int max_x = std::max(first.first, second.first);
    int min_x = std::min(first.first, second.first);
    int max_y = std::max(first.second, second.second);
    int min_y = std::min(first.second, second.second);
    upperLeft = std::make_pair(min_x, min_y); 
    lowerRight = std::make_pair(max_x, max_y); 
}

std::vector<std::pair<int, int>> Rectangle::getBorderCoordinates() {
    std::vector<std::pair<int, int>> temp;
    for (int i = upperLeft.first; i <= lowerRight.first; i++) { 
        temp.emplace_back(i, lowerRight.second); 
        temp.emplace_back(i, upperLeft.second);  
    }

    for(int i = upperLeft.second; i <= lowerRight.second; i++) { 
        temp.emplace_back(upperLeft.first, i);  
        temp.emplace_back(lowerRight.first, i); 
    }

    return temp;
}

std::vector<std::pair<int, int>> Rectangle::getAreaCoordinates() {
    std::vector<std::pair<int, int>> result;
    for (int y = upperLeft.second + 1; y < lowerRight.second; y++) {
        for (int x = upperLeft.first + 1; x < lowerRight.first; x++) {
            result.emplace_back(x, y);
        }
    }
    return result;
}

std::vector<std::pair<int, int>> Rectangle::getBorderCoordinates(int thickness) {
    std::vector<std::pair<int, int>> result;
    std::pair<int, int> currentRight = std::pair<int, int>(lowerRight.first, upperLeft.second);
    std::pair<int, int> currentLeft = std::pair<int, int>(upperLeft.first, lowerRight.second);
    Shape* circle_upperLeft = new Circle(upperLeft, thickness - 1);
    Shape* circle_lowerRight = new Circle(lowerRight, thickness - 1);
    Shape* circle_currentRight = new Circle(currentRight, thickness - 1);
    Shape* circle_currentLeft = new Circle(currentLeft, thickness - 1);
    std::vector<std::pair<int, int>> circle_one_border = circle_upperLeft->getBorderCoordinates();
    std::vector<std::pair<int, int>> circle_two_border = circle_lowerRight->getBorderCoordinates();
    std::vector<std::pair<int, int>> circle_three_border = circle_currentRight->getBorderCoordinates();
    std::vector<std::pair<int, int>> circle_four_border = circle_currentLeft->getBorderCoordinates();
    std::vector<std::pair<int, int>> circle_one = circle_upperLeft->getAreaCoordinates();
    std::vector<std::pair<int, int>> circle_two = circle_lowerRight->getAreaCoordinates();
    std::vector<std::pair<int, int>> circle_three = circle_currentRight->getAreaCoordinates();
    std::vector<std::pair<int, int>> circle_four = circle_currentLeft->getAreaCoordinates();
    std::pair<int, int> coord_rect_one_left_up = std::pair<int, int>(upperLeft.first - thickness, upperLeft.second);
    std::pair<int, int> coord_rect_one_right_down = std::pair<int, int>(currentLeft.first + thickness, currentLeft.second);
    std::pair<int, int> coord_rect_two_left_up = std::pair<int, int>(upperLeft.first, upperLeft.second + thickness);
    std::pair<int, int> coord_rect_two_right_down = std::pair<int, int>(currentRight.first, currentRight.second - thickness);
    std::pair<int, int> coord_rect_three_left_up = std::pair<int, int>(currentRight.first - thickness, currentRight.second);
    std::pair<int, int> coord_rect_three_right_down = std::pair<int, int>(lowerRight.first + thickness, lowerRight.second);
    std::pair<int, int> coord_rect_four_left_up = std::pair<int, int>(currentLeft.first, currentLeft.second - thickness);
    std::pair<int, int> coord_rect_four_right_down = std::pair<int, int>(lowerRight.first, lowerRight.second + thickness);
    Shape* rect_one = new Rectangle(coord_rect_one_left_up, coord_rect_one_right_down);
    Shape* rect_two = new Rectangle(coord_rect_two_left_up, coord_rect_two_right_down);
    Shape* rect_three = new Rectangle(coord_rect_three_left_up, coord_rect_three_right_down);
    Shape* rect_four = new Rectangle(coord_rect_four_left_up, coord_rect_four_right_down);
    std::vector<std::pair<int, int>> rect_one_coordinates = rect_one->getAreaCoordinates();
    std::vector<std::pair<int, int>> rect_two_coordinates = rect_two->getAreaCoordinates();
    std::vector<std::pair<int, int>> rect_three_coordinates = rect_three->getAreaCoordinates();
    std::vector<std::pair<int, int>> rect_four_coordinates = rect_four->getAreaCoordinates();
    result.insert(result.end(), rect_one_coordinates.begin(), rect_one_coordinates.end());
    result.insert(result.end(), rect_two_coordinates.begin(), rect_two_coordinates.end());
    result.insert(result.end(), rect_three_coordinates.begin(), rect_three_coordinates.end());
    result.insert(result.end(), rect_four_coordinates.begin(), rect_four_coordinates.end());
    result.insert(result.end(), circle_one_border.begin(), circle_one_border.end());
    result.insert(result.end(), circle_two_border.begin(), circle_two_border.end());
    result.insert(result.end(), circle_three_border.begin(), circle_three_border.end());
    result.insert(result.end(), circle_four_border.begin(), circle_four_border.end());
    result.insert(result.end(), circle_one.begin(), circle_one.end());
    result.insert(result.end(), circle_two.begin(), circle_two.end());
    result.insert(result.end(), circle_three.begin(), circle_three.end());
    result.insert(result.end(), circle_four.begin(), circle_four.end());
    circle_one.clear();
    circle_two.clear();
    circle_three.clear();
    circle_four.clear();
    rect_one_coordinates.clear();
    rect_two_coordinates.clear();
    rect_three_coordinates.clear();
    rect_four_coordinates.clear();
    delete circle_upperLeft;
    delete circle_lowerRight;
    delete circle_currentLeft;
    delete circle_currentRight;
    delete rect_one;
    delete rect_two;
    delete rect_three;
    delete rect_four;
    return result;
}

Polygon::Polygon(std::pair<int, int> center, int radius, int corners) {
    this->center = center;
    this->radius = radius;
    this->corners = corners;
}

std::vector<std::pair<int, int>> Polygon::getCornersCoordinates() {
    std::vector<std::pair<int, int>> cornersCoordinates;
    for (int i = 0; i < corners;i++){
        cornersCoordinates.emplace_back(center.first + radius * std::cos(i * (2 * M_PI / corners)), center.second + radius * std::sin(i * (2 * M_PI / corners)));
    }
    cornersCoordinates.emplace_back(center.first + radius * std::cos(0 * (2 * M_PI / corners)), center.second + radius * std::sin(0 * (2 * M_PI / corners)));
    return cornersCoordinates;
}

std::vector<std::pair<int, int>> Polygon::getBorderCoordinates() {
    std::vector<std::pair<int, int>> cornersCoordinates = getCornersCoordinates();
    std::vector<std::pair<int, int>> result;
    for (int i = 0; i < corners;i++){
        Shape* line = new Line(cornersCoordinates[i], cornersCoordinates[i + 1]);
        std::vector<std::pair<int, int>> linePoints = line->getBorderCoordinates();
        result.insert(result.end(),linePoints.begin(), linePoints.end());
        linePoints.clear();
        delete line;
    }
    return result;
}

std::vector<std::pair<int, int>> Polygon::getAreaCoordinates() {
    std::vector<std::pair<int, int>> result;
    std::vector<std::pair<int, int>> cornerCoordinates = getCornersCoordinates();
    std::vector<std::pair<int, int>> borderCoordinates = getBorderCoordinates();
    std::pair<int, int> borders_y = std::make_pair(INT_MAX, INT_MIN);
    for (std::pair<int, int> coordinate : cornerCoordinates)
    {
        borders_y.first = std::min(borders_y.first, coordinate.second);
        borders_y.second = std::max(borders_y.second, coordinate.second);
    }
    for (int y = borders_y.first + 1; y < borders_y.second; y++)
    {
        std::pair<int, int> borders_x = std::make_pair(INT_MAX, INT_MIN);
        for (std::pair<int, int> coordinate : borderCoordinates)
        {
            if (coordinate.second == y)
            {
                borders_x.first = std::min(borders_x.first, coordinate.first);
                borders_x.second = std::max(borders_x.second, coordinate.first);
                for (int x = borders_x.first + 1; x < borders_x.second; x++)
                {
                    result.emplace_back(x, y);
                }
            }
        }
        for (int x = borders_x.first + 1; x < borders_x.second; x++)
        {
            result.emplace_back(x, y);
        }
    }
    return result;
}

void description(){
    std::cout << "Course work for option 4.21, created by Vladislav Trofimov." << std::endl;
}

std::pair<int, int> processCoordinatesCli(std::string string_coordinates) {
    std::pair<int, int> coordinates;
    std::cmatch groups;
    std::regex pattern("(-?\\d+)\\.(-?\\d+)");
    if (!std::regex_match(string_coordinates.c_str(), groups, pattern)) {
        std::cout << "CLI Error: Coordinates not matched x.y" << std::endl;
        exit(40);
    } 
    coordinates = {std::stoi(groups[1]) , std::stoi(groups[2])};
    return coordinates;
}

int processNumberCli(std::string string_number) {
    std::cmatch groups;
    std::regex pattern("(-?\\d+)");
    if (!std::regex_match(string_number.c_str(), groups, pattern)) {
        std::cout << "CLI Error: Number not matched with format" << std::endl;
        exit(40);
    } 
    int number = std::stoi(groups[1]);
    if (number <= 0) {
        std::cout << "CLI Error: Number not processed" << std::endl;
        exit(40);
    }
    return number;
}

void processColorCli(Rgbacolour& color,std::string string_color) {
    std::cmatch groups;
    std::regex pattern("(\\d+)\\.(\\d+)\\.(\\d+)");
    if (!std::regex_match(string_color.c_str(), groups, pattern)) {
        std::cout << "CLI Error: Colour not matched with format rrr.ggg.bbb" << std::endl;
        exit(40);
    }
    color.red = std::stoi(groups[1]);
    color.green = std::stoi(groups[2]);
    color.blue = std::stoi(groups[3]);
    color.alpha = 255;
    if ((0 > color.red || color.red > 255) || (0 > color.green || color.green > 255) || (0 > color.blue || color.blue > 255)){
        std::cout << "CLI Error: Color value does not match the expected format" << std::endl;
        exit(40);
    }
}

std::set<int> getUnusableFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> functionalFlags) {
    std::set<int> result;
    for (auto i : flags){
        if (necessaryFlags.find(i.first) == necessaryFlags.end() && functionalFlags.find(i.first) == functionalFlags.end()){
            result.insert(i.first);
        }
    }
    return result;
}

bool checkValidFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> unusableFlags) {
    int all_necessaryFlags = 0;
    for (auto i : necessaryFlags){
        if(flags[i].inputed) {
            all_necessaryFlags++;
        }
    }

    int all_unusableFlags = 0;
    for (auto i : unusableFlags) {
        if(!flags[i].inputed) {
            all_unusableFlags++;
        }
    }

    return (all_necessaryFlags == (int)necessaryFlags.size() && all_unusableFlags == (int)unusableFlags.size());
}

void proccessFileOutname(std::map<int, Flag> flags, std::string& inputFilename, std::string& outputFilename, std::string lastArgument) {
    if (flags[INPUT_INDEX].inputed) {
        inputFilename = flags[INPUT_INDEX].value;
    } else {
        inputFilename = lastArgument;
    }
    if (flags[OUTPUT_INDEX].inputed){
        outputFilename = flags[OUTPUT_INDEX].value;
    }
}

void drawRectangle(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> left_up = processCoordinatesCli(flags[LEFT_UP_INDEX].value);
    std::pair<int, int> right_down = processCoordinatesCli(flags[RIGHT_DOWN_INDEX].value);
    Rgbacolour fill_color(0,0,0);
    Rgbacolour color(0,0,0);
    int thickness = processNumberCli(flags[THICKNESS_INDEX].value);
    processColorCli(color, flags[COLOR_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags, inputFilename, outputFilename, lastArgument);
    Shape* current_shape = new Rectangle(left_up, right_down);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    std::vector<std::pair<int, int>> coordinates = current_shape->getBorderCoordinates(thickness);
    std::vector<std::pair<int, int>> coordinates_filled = current_shape->getAreaCoordinates();
    bool fill = flags[FILL_INDEX].inputed;
    if (fill) {
        processColorCli(fill_color, flags[FILL_COLOR_INDEX].value);
        for (std::pair<int, int> coordinate : coordinates) {
            image.savePixel(coordinate, color);
        }
        for (std::pair<int, int> coordinate : coordinates_filled) {
            image.savePixel(coordinate, fill_color);
        }
    }
    else {
        for (std::pair<int, int> coordinate : coordinates) {
            image.savePixel(coordinate, color);
        }
    }
    image.write(outputFilename.c_str());
    delete current_shape;
}

void drawHexagon(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> center = processCoordinatesCli(flags[CENTER_INDEX].value);
    int radius = processNumberCli(flags[RADIUS_INDEX].value);
    int thickness = processNumberCli(flags[THICKNESS_INDEX].value);
    Rgbacolour color(0,0,0);
    Rgbacolour fill_color(0,0,0);
    processColorCli(color, flags[COLOR_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags,inputFilename, outputFilename, lastArgument);
    Shape* current_shape = new Polygon(center, radius, 6);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    std::vector<std::pair<int, int>> coordinates = current_shape->getBorderCoordinates(thickness);
    std::vector<std::pair<int, int>> coordinates_filled = current_shape->getAreaCoordinates();
    bool fill = flags[FILL_INDEX].inputed;
    if (fill) {
        processColorCli(fill_color, flags[FILL_COLOR_INDEX].value);
        for (std::pair<int, int> coordinate : coordinates_filled) {
            image.savePixel(coordinate, fill_color);
        }
        for (std::pair<int, int> coordinate : coordinates) {
            image.savePixel(coordinate, color);
        }
    }
    else {
        for (std::pair<int, int> coordinate : coordinates) {
            image.savePixel(coordinate, color);
        }
    }
    image.write(outputFilename.c_str());
    delete current_shape;
} 

void copyArea(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> left_up = processCoordinatesCli(flags[LEFT_UP_INDEX].value);
    std::pair<int, int> right_down = processCoordinatesCli(flags[RIGHT_DOWN_INDEX].value);
    std::pair<int, int> dest_left_up = processCoordinatesCli(flags[DEST_LEFT_UP_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags, inputFilename, outputFilename, lastArgument);
    ICopy* current_shape = new ICopy(left_up, right_down, dest_left_up);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    std::vector<std::pair<int, int>> coordinates = current_shape->getSourceCoordiantes();
    std::vector<std::pair<int, int>> coordinates_d = current_shape->getDestinationCoordinates();
    std::vector<Rgbacolour> colours;
    for (std::pair<int, int> coordinate : coordinates)
    {   
        colours.push_back(image.getPixel(coordinate));
    }
    for (int i = 0; i < (int)coordinates_d.size();i++)
    {   
        image.savePixel(coordinates_d[i], colours[i]);
    }
    image.write(outputFilename.c_str());
    delete current_shape;
}

void helpUser(){
    std::cout << "Программа должна реализовывать весь следующий функционал по обработке png-файла." << std::endl
          << "Программа должна иметь следующую функции по обработке изображений: " << std::endl
          << "(1) Рисование прямоугольника. Флаг для выполнения данной операции: `--rect`. Он определяется:" << std::endl
          << "Координатами левого верхнего угла. Флаг `--left_up`, значение задаётся в формате `left.up`, где left – координата по x,"
          << " up – координата по y. Координатами правого нижнего угла.  Флаг `--right_down`, значение задаётся в формате `right.down`,"
          << " где right – координата по x, down – координата по y." << std::endl
          << "Толщиной линий: флаг `--thickness`. На вход принимает число больше 0." << std::endl
          << "Цветом линий. Флаг `--color` (цвет задаётся строкой `rrr.ggg.bbb`, где rrr/ggg/bbb – числа, задающие цветовую компоненту."
          << " пример `--color 255.0.0` задаёт красный цвет)." << std::endl
          << "Прямоугольник может быть залит или нет. Флаг `--fill`. Работает как бинарное значение: флага нет – false , флаг есть – true."
          << " цветом которым он залит, если пользователем выбран залитый. Флаг `--fill_color` (работает аналогично флагу `--color`)." << std::endl
          << "(2) Рисование правильного шестиугольника. Флаг для выполнения данной операции: `--hexagon`. Шестиугольник определяется:"
          << std::endl << "координатами его центра и радиусом в который он вписан. Флаги `--center` и `--radius`. Значение флаг `--center`"
          << " задаётся в формате `x.y`, где x – координата по оси x, y – координата по оси y." << std::endl
          << "Флаг `--radius` На вход принимает число больше 0." << std::endl
          << "Толщиной линий. Флаг `--thickness`. На вход принимает число больше 0 цветом линий." << std::endl
          << "Флаг `--color` (цвет задаётся строкой `rrr.ggg.bbb`, где rrr/ggg/bbb – числа,задающие цветовую компоненту."
          << " пример `--color 255.0.0` задаёт красный цвет)." << std::endl
          << "Шестиугольник может быть залит или нет. Флаг `--fill`. Работает как бинарное значение: флага нет – false , флаг есть – true."
          << " цветом которым залит шестиугольник, если пользователем выбран залитый. Флаг `--fill_color` (работает аналогично флагу `--color`)."
          << std::endl << "(3) Копирование заданной области. Флаг для выполнения данной операции: `--copy`. Функционал определяется:"
          << std::endl << "Координатами левого верхнего угла области-источника. Флаг `--left_up`, значение задаётся в формате `left.up`, где left – координата"
          << " по x, up – координата по y " << std::endl << "Координатами правого нижнего угла области-источника. Флаг `--right_down`, значение"
          << " задаётся в формате `right.down`, где right – координата по x, down – координата по y" << std::endl
          << "Координатами левого верхнего угла области-назначения. Флаг `--dest_left_up`, значение задаётся в формате `left.up`,"
          << " где left – координата по x, up – координата по y." << std::endl;
}

void infoImage(std::map<int, Flag> flags, std::string lastArgument){
    ImageFileReader image;
    std::string inputFilename;
    std::string outputFilename;
    proccessFileOutname(flags,inputFilename, outputFilename, lastArgument);
    image.resolve(inputFilename.c_str());
    std:: cout << "Высота изображения " << image.getHeight() << std::endl 
            <<"Ширина изображения " << image.getWidth() << std::endl
            <<"Глубина изображения " << image.getBitDepth() << " в байтах" << std::endl
            <<"Тип цвета изображения " << image.getColortype() << std::endl;
}

bool flagsRectangle(std::map<int, Flag> flags) {
    std::set<int> requiredFlags = {RECT_INDEX,LEFT_UP_INDEX,RIGHT_DOWN_INDEX, THICKNESS_INDEX, COLOR_INDEX};
    std::set<int> functionalFlags = {INPUT_INDEX, OUTPUT_INDEX, FILL_INDEX, FILL_COLOR_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, requiredFlags, functionalFlags);
    return checkValidFlags(flags, requiredFlags, unusableFlags) && !(flags[FILL_INDEX].inputed && !flags[FILL_COLOR_INDEX].inputed);
}

bool flagsHexagon(std::map<int, Flag> flags) {
    std::set<int> necessaryFlags = {HEXAGON_INDEX, CENTER_INDEX, RADIUS_INDEX, THICKNESS_INDEX, COLOR_INDEX};
    std::set<int> functionalFlags = {INPUT_INDEX, OUTPUT_INDEX, FILL_INDEX, FILL_COLOR_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, necessaryFlags, functionalFlags);
    return checkValidFlags(flags, necessaryFlags, unusableFlags) && !(flags[FILL_INDEX].inputed && !(flags[FILL_COLOR_INDEX].inputed));
}

bool flagsCopy(std::map<int, Flag> flags) {
    std::set<int> necessaryFlags = {COPY_INDEX, LEFT_UP_INDEX, RIGHT_DOWN_INDEX, DEST_LEFT_UP_INDEX};
    std::set<int> functionalFlags = {INPUT_INDEX, OUTPUT_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, necessaryFlags, functionalFlags);
    return checkValidFlags(flags, necessaryFlags, unusableFlags);
}

bool flagsHelp(std::map<int, Flag> flags) {
    std::set<int> necessaryFlags = {};
    std::set<int> functionalFlags = {HELP_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, necessaryFlags, functionalFlags);
    return checkValidFlags(flags, necessaryFlags, unusableFlags);
}

bool flagsInfo(std::map<int, Flag> flags) {
    std::set<int> necessaryFlags = {INFO_INDEX};
    std::set<int> functionalFlags = {INPUT_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, necessaryFlags, functionalFlags);
    return checkValidFlags(flags, necessaryFlags, unusableFlags);
}
void selectionFunction(std::map<int, Flag> flags, std::string lastArgument){
    if (flagsRectangle(flags)){
        drawRectangle(flags, lastArgument);
    }
    else if (flagsHexagon(flags)){
        drawHexagon(flags, lastArgument);
    }
    else if (flagsCopy(flags)) {
        copyArea(flags, lastArgument);
    }
    else if (flagsHelp(flags)){
        helpUser();
    }
    else if (flagsInfo(flags)){
        infoImage(flags, lastArgument);
    }
    else {
        std::cout << "Error CLI: Unknown function" << std::endl;
        exit(40);
    }
}

void processCLI(int argc, char** argv) {
    std::map<int, Flag> flags = {
        {INPUT_INDEX, {0, ""}},
        {OUTPUT_INDEX, {0, ""}},
        {HELP_INDEX, {0, ""}},
        {INFO_INDEX, {0, ""}},
        {RECT_INDEX, {0, ""}},
        {LEFT_UP_INDEX, {0, ""}},
        {RIGHT_DOWN_INDEX, {0, ""}},
        {THICKNESS_INDEX, {0, ""}},
        {COLOR_INDEX, {0, ""}},
        {FILL_INDEX, {0, ""}},
        {FILL_COLOR_INDEX, {0, ""}},
        {HEXAGON_INDEX, {0, ""}},
        {CENTER_INDEX, {0, ""}},
        {RADIUS_INDEX, {0, ""}},
        {DEST_LEFT_UP_INDEX, {0, ""}},
        {COPY_INDEX, {0, ""}}
    };
    const char* short_options = "hi:o:";
    const option long_options[] = {
        {"input", required_argument, nullptr, INPUT_INDEX},
        {"output", required_argument, nullptr, OUTPUT_INDEX},
        {"help", no_argument, nullptr, HELP_INDEX},
        {"info", no_argument, nullptr, INFO_INDEX},
        {"rect", no_argument, nullptr, RECT_INDEX},
        {"left_up", required_argument, nullptr, LEFT_UP_INDEX},
        {"right_down", required_argument, nullptr, RIGHT_DOWN_INDEX},
        {"thickness", required_argument, nullptr, THICKNESS_INDEX},
        {"color", required_argument, nullptr, COLOR_INDEX},
        {"fill", no_argument, nullptr, FILL_INDEX},
        {"fill_color", required_argument, nullptr, FILL_COLOR_INDEX},
        {"hexagon", no_argument, nullptr, HEXAGON_INDEX},
        {"center", required_argument, nullptr, CENTER_INDEX},
        {"radius", required_argument, nullptr, RADIUS_INDEX},
        {"dest_left_up", required_argument, nullptr, DEST_LEFT_UP_INDEX},
        {"copy", no_argument, nullptr, COPY_INDEX},
        {nullptr, 0, nullptr, 0}
    };
    int option;
    while ((option = getopt_long(argc, argv, short_options, long_options, NULL)) != -1) {
        if (flags.find(option) != flags.end()){
            flags[option].inputed = true;
            if (optarg) {
                flags[option].value = optarg;
            }
        }
        else {
            std::cout << "Error CLI: wrong flag" << std::endl;
            exit(40);
        }
    }
    std::string lastArgument = argv[argc - 1];
    selectionFunction(flags, lastArgument);
}

int main(int argc, char** argv) {
    description();
    processCLI(argc, argv);
    return 0;
}
