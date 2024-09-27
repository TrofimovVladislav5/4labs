#include <getopt.h>
#include "ProcessCliValues.h"
#include "Functions.h"
#include "Flags.h"
#include <regex>
#include <map>

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

float processFloatNumberCli(std::string string_number) {
    std::cmatch groups;
    std::regex pattern("(-?\\d+)\\.(-?\\d+)");
    if (!std::regex_match(string_number.c_str(), groups, pattern)) {
        std::cout << "CLI Error: Number not matched with format" << std::endl;
        exit(40);
    } 
    float number = std::stof(groups[0]);
    return number;
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

int processNumberCliWithNegativeValues(std::string string_number) {
    std::cmatch groups;
    std::regex pattern("(-?\\d+)");
    if (!std::regex_match(string_number.c_str(), groups, pattern)) {
        std::cout << "CLI Error: Number not matched with format" << std::endl;
        exit(40);
    } 
    int number = std::stoi(groups[1]);
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

void selectionFunction(std::map<int, Flag> flags, std::string lastArgument){
    if (flagsRectangle(flags)) {
        drawRectangle(flags, lastArgument);
    }
    else if (flagsHexagon(flags)){
        drawHexagon(flags, lastArgument);
    }
    else if (flagsCopy(flags)) {
        copyArea(flags, lastArgument);
    }
    else if (flagsHelp(flags)) {
        helpUser();
    }
    else if (flagsInfo(flags)){
        infoImage(flags, lastArgument);
    }
    else if (flagsBitwise(flags)){
        bitwise(flags, lastArgument);
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
        {COPY_INDEX, {0, ""}},
        {OP, {0, ""}},
        {BITWISE_THRESHOLD, {0, ""}},
        {THRESHOLD_RED, {0, ""}},
        {THRESHOLD_BLUE, {0, ""}},
        {THRESHOLD_GREEN, {0, ""}},
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
        {"op", required_argument, nullptr, OP},
        {"bitwise_threshold", no_argument,nullptr, BITWISE_THRESHOLD},
        {"threshold_red", required_argument, nullptr, THRESHOLD_RED},
        {"threshold_blue", required_argument, nullptr, THRESHOLD_BLUE},
        {"threshold_green", required_argument, nullptr, THRESHOLD_GREEN},
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