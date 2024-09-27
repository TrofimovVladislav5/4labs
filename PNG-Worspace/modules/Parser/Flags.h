#ifndef FLAGS_H
#define FLAGS_H
#include <map>
#include <set>
#include "Structures.h"

bool flagsRectangle(std::map<int, Flag> flags);
bool flagsHexagon(std::map<int, Flag> flags);
bool flagsCopy(std::map<int, Flag> flags);
bool flagsHelp(std::map<int, Flag> flags);
bool flagsInfo(std::map<int, Flag> flags);
bool flagsBitwise(std::map<int, Flag> flags);
std::set<int> getUnusableFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> functionalFlags);
bool checkValidFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> unusableFlags);
#endif