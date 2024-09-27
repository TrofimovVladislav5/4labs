#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Structures.h"
#include "Indexes.h"
#include "ImageFileReader.h"
#include <getopt.h>

void drawRectangle(std::map<int, Flag> flags, std::string lastArgument);
void drawHexagon(std::map<int, Flag> flags, std::string lastArgument);
void copyArea(std::map<int, Flag> flags, std::string lastArgument);
void bitwise(std::map<int, Flag> flags, std::string lastArgument);
void helpUser();
void infoImage(std::map<int, Flag> flags, std::string lastArgument);
void description();

#endif