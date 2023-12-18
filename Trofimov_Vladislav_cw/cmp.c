#include <wchar.h>
#include "structures.h"
#include <stdlib.h>

int cmp(const void* a, const void* b){
    Sentence* first = (Sentence*)a;
    Sentence* second = (Sentence*)b;

    return first->wordCount - second->wordCount;
}