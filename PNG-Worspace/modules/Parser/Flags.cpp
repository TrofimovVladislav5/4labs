#include <map>
#include "Indexes.h"
#include "Structures.h"
#include "Flags.h" 

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

bool flagsBitwise(std::map<int, Flag> flags) {
    std::set<int> necessaryFlags = {BITWISE_THRESHOLD, OP, THRESHOLD_RED, THRESHOLD_BLUE, THRESHOLD_GREEN};
    std::set<int> functionalFlags = {INPUT_INDEX, OUTPUT_INDEX};
    std::set<int> unusableFlags = getUnusableFlags(flags, necessaryFlags, functionalFlags);
    return checkValidFlags(flags, necessaryFlags, unusableFlags);
}

std::set<int> getUnusableFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> functionalFlags) {
    std::set<int> result;
    for (auto i : flags){
        if (necessaryFlags.find(i.first) == necessaryFlags.end() && functionalFlags.find(i.first) == functionalFlags.end()) {
            result.insert(i.first);
        }
    }
    return result;
}

bool checkValidFlags(std::map<int, Flag> flags, std::set<int> necessaryFlags, std::set<int> unusableFlags) {
    int all_necessaryFlags = 0;
    for (auto i : necessaryFlags) {
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