#include "help.h"
#include "commandError.h"

int checkPresentText(int value){
    if (value == 5){
        help();
        return 0;
    }
    if (value >= 0 && value < 5){
        return 1;
    }
    commandError();
    return 0;
}