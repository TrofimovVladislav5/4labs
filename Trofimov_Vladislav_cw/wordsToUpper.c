#include <wchar.h>
#include <stdlib.h>
#include <wctype.h>


void wordToUpper(wchar_t* string) {
    for (int i = 0; i < wcslen(string);i++){
        string[i] = towupper(string[i]);
    }
}