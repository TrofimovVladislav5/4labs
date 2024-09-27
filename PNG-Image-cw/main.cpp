#include "ImageFileReader.h"
#include "Flags.h"
#include "Functions.h"
#include "Indexes.h"
#include "ProcessCliValues.h"
#include "Structures.h"


int main(int argc, char** argv) {
    description();
    processCLI(argc, argv);
    return 0;
}