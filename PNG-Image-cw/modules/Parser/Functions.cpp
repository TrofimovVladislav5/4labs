#include "Indexes.h"
#include "Flags.h"
#include "ProcessCliValues.h"
#include "Structures.h"
#include "ImageFileReader.h"
#include "Functions.h"

void drawRectangle(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> left_up = processCoordinatesCli(flags[LEFT_UP_INDEX].value);
    std::pair<int, int> right_down = processCoordinatesCli(flags[RIGHT_DOWN_INDEX].value);
    Rgbacolour fill_color;
    Rgbacolour color;
    int thickness = processNumberCli(flags[THICKNESS_INDEX].value);
    processColorCli(color, flags[COLOR_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags, inputFilename, outputFilename, lastArgument);
    bool fill = flags[FILL_INDEX].inputed;
    if (fill) 
        processColorCli(fill_color, flags[FILL_COLOR_INDEX].value);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    image.drawBorderRect(left_up.first, left_up.second, right_down.first, right_down.second, color, thickness, fill, fill_color);
    image.write(outputFilename.c_str());
}

void drawHexagon(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> center = processCoordinatesCli(flags[CENTER_INDEX].value);
    int radius = processNumberCli(flags[RADIUS_INDEX].value);
    int thickness = processNumberCli(flags[THICKNESS_INDEX].value);
    Rgbacolour color;
    Rgbacolour fill_color;
    processColorCli(color, flags[COLOR_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags,inputFilename, outputFilename, lastArgument);
    bool fill = flags[FILL_INDEX].inputed;
    if (fill) 
        processColorCli(fill_color, flags[FILL_COLOR_INDEX].value);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    image.drawHexagon(center.first, center.second, radius, color, thickness, fill, fill_color);
    image.write(outputFilename.c_str());
} 

void copyArea(std::map<int, Flag> flags, std::string lastArgument) {
    std::pair<int, int> left_up = processCoordinatesCli(flags[LEFT_UP_INDEX].value);
    std::pair<int, int> right_down = processCoordinatesCli(flags[RIGHT_DOWN_INDEX].value);
    std::pair<int, int> dest_left_up = processCoordinatesCli(flags[DEST_LEFT_UP_INDEX].value);
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags, inputFilename, outputFilename, lastArgument);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    image.copyArea(left_up.first,left_up.second,right_down.first, right_down.second, dest_left_up.first, dest_left_up.second);
    image.write(outputFilename.c_str());
}

void bitwise(std::map<int, Flag> flags, std::string lastArgument){
    int threshold_red = processNumberCliWithNegativeValues(flags[THRESHOLD_RED].value);
    int threshold_blue = processNumberCliWithNegativeValues(flags[THRESHOLD_BLUE].value);
    int threshold_green = processNumberCliWithNegativeValues(flags[THRESHOLD_GREEN].value);
    std::string op = flags[OP].value;
    if (op != "and" && op != "xor"){
        exit(40);
    }
    std::string inputFilename;
    std::string outputFilename = "out.png";
    proccessFileOutname(flags, inputFilename, outputFilename, lastArgument);
    ImageFileReader image;
    image.resolve(inputFilename.c_str());
    image.bitwiseImage(op, threshold_red, threshold_green, threshold_blue);
    image.write(outputFilename.c_str());
}

void helpUser() {
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

void infoImage(std::map<int, Flag> flags, std::string lastArgument) {
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

void description(){
    std::cout << "Course work for option 4.21, created by Vladislav Trofimov." << std::endl;
}