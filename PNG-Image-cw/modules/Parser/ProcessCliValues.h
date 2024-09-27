#ifndef PROCESSCLIVALUES_H
#define PROCESSCLIVALUES_H

#include <getopt.h>
#include "Structures.h"
#include "Indexes.h"

std::pair<int, int> processCoordinatesCli(std::string string_coordinates);
float processFloatNumberCli(std::string string_number);
int processNumberCli(std::string string_number);
int processNumberCliWithNegativeValues(std::string string_number);
void processColorCli(Rgbacolour& color,std::string string_color);
void selectionFunction(std::map<int, Flag> flags, std::string lastArgument);
void processCLI(int argc, char** argv);
void proccessFileOutname(std::map<int, Flag> flags, std::string& inputFilename, std::string& outputFilename, std::string lastArgument);
#endif