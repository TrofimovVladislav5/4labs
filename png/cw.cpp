#include <iostream>
#include <vector>
#include <cmath>
#include <png.h>

struct Rgbacolour{
    png_byte red;
    png_byte green;
    png_byte blue;
    png_byte alpha;

    Rgbacolour(png_byte r, png_byte g, png_byte b, png_byte a = 255){
        red = r;
        green = g;
        blue = b;
        alpha = a;
    }
};

class ImageFileReader {
private:
    uint32_t width;
    uint32_t height;
    png_byte color_type;
    png_byte bit_depth;
    int number_of_passes;
    png_structp png_ptr;
    png_infop info_ptr;
    png_bytep *row_pointers;

    void allocateRowPointers() {
        row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
        for (int y = 0; y < height; y++) {
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr, info_ptr));
        }
    }

    static void checkFileHeader(FILE* file) {
        png_byte header[8];
        fread(header, sizeof(png_byte), 8, file);
        if (png_sig_cmp(header, 0, 8)){
            std:: cerr << "Error: File isn't have png format" << std::endl;
            return;
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
        checkFileHeader(input_png);
        png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
        info_ptr = png_create_info_struct(png_ptr);
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

    void write(const char* filename) {
        FILE *output_png = fopen(filename, "wb");
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

    void savePixel(std::pair<int, int> coordinates, Rgbacolour colour) {
        if (coordinates.first >= 0 && coordinates.first < width && coordinates.second >= 0 && coordinates.second < height) {
            png_bytep pixel = &(row_pointers[coordinates.second][coordinates.first * 4]);
            pixel[0] = colour.red;
            pixel[1] = colour.green;
            pixel[2] = colour.blue;
            pixel[3] = colour.alpha;
        }
    }
    
    Rgbacolour getPixel(std::pair<int, int> coordinates) {
        Rgbacolour colourPixel(0,0,0);
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
    virtual std::vector<std::pair<int, int>> getBorderCoordinates() override;
    std::vector<std::pair<int, int>> getBorderCoordinates(int) override;
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


// std::vector<std::pair<int, int>> Circle::getAreaCoordinates() {
//     std::vector<std::pair<int, int>> result;
//     std::vector<std::pair<int, int>> borderCoordinates = getBorderCoordinates();
//     std::pair<int, int> borders_y = std::make_pair(INT_MAX, INT_MIN);
//     for (std::pair<int, int> coordinate : borderCoordinates)
//     {
//         borders_y.first = std::min(borders_y.first, coordinate.second);
//         borders_y.second = std::max(borders_y.second, coordinate.second);
//     }
//     for (int y = borders_y.first + 1; y < borders_y.second; y++)
//     {
//         std::pair<int, int> borders_x = std::make_pair(INT_MAX, INT_MIN);
//         for (std::pair<int, int> coordinate : borderCoordinates)
//         {
//             if (coordinate.second == y)
//             {
//                 borders_x.first = std::min(borders_x.first, coordinate.first);
//                 borders_x.second = std::max(borders_x.second, coordinate.first);
//             }
//             for (int x = borders_x.first + 1; x < borders_x.second; x++)
//             {
//                 result.emplace_back(x, y);
//             }
//         }
//     }
//     return result;
// }

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

    for(int i = lowerRight.second; i <= upperLeft.second; i++) { 
        temp.emplace_back(lowerRight.first, i); 
        temp.emplace_back(upperLeft.first, i);  
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

int main() {
    const char *input_file = "Copy.png";
    const char *output_file = "output.png";
    const Rgbacolour draw_colour(255, 0, 0);
    const Rgbacolour circle_colour(0, 0, 255);
    std::pair<int, int> center = std::pair<int, int>(200, 100);
    Shape* circle = new Circle(center, 300);
    ImageFileReader image;
    image.resolve(input_file);
    std::vector<std::pair<int, int>> coordinates = circle->getBorderCoordinates(500);
    std::vector<std::pair<int, int>> coordinates_f = circle->getAreaCoordinates();
    for (std::pair<int, int> coordinate : coordinates)
    {   
        image.savePixel(coordinate, draw_colour);
    }
    for (std::pair<int, int> coordinate : coordinates_f)
    {   
        image.savePixel(coordinate, circle_colour);
    }
    image.write(output_file);
    delete circle;
    return 0;
}

// // int main() { под ICopy
//     const char *input_file = "Copy.png";
//     const char *output_file = "output.png";
//     const Rgbacolour draw_colour(255, 0, 0);
//     const Rgbacolour circle_colour(0, 0, 255);
//     std::pair<int, int> uppers = std::pair<int, int>(100,50);
//     std::pair<int, int> lowerrs = std::pair<int, int>(200, 100);
//     std::pair<int, int> destination = std::pair<int, int>(0, 50);
//     ICopy* rect = new ICopy(uppers, lowerrs, destination);
//     ImageFileReader image;
//     image.resolve(input_file);
//     std::vector<std::pair<int, int>> coordinates = rect->getSourceCoordiantes();
//     std::vector<std::pair<int, int>> coordinates_d = rect->getDestinationCoordinates();
//     std::vector<Rgbacolour> colours;
//     for (std::pair<int, int> coordinate : coordinates)
//     {   
//         image.savePixel(coordinate, draw_colour);
//         colours.push_back(image.getPixel(coordinate));
//     }
//     for (int i = 0; i < coordinates_d.size();i++)
//     {   
//         printf("%d %d %d %d\n", colours[i].red, colours[i].green, colours[i].blue, colours[i].alpha);
//         image.savePixel(coordinates_d[i], colours[i]);
//     }
//     image.write(output_file);
//     delete rect;
//     return 0;
// }