#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include "structures.h"
#include "wordsToUpper.h"

void deleteDuplicates(Text* text) {
    wchar_t* copy_i;
    wchar_t* copy_j;
    for (int i = 0; i < text->count; i++) {
        if (text->sentences[i].str != NULL) {
            wchar_t* copy_i = (wchar_t*) calloc(wcslen(text->sentences[i].str) + 1, sizeof(wchar_t));
            wcscpy(copy_i, text->sentences[i].str);
            wordToUpper(copy_i);
            for (int j = i + 1; j < text->count; j++) {
                if (text->sentences[j].str != NULL) {
                    wchar_t* copy_j = (wchar_t*) calloc(wcslen(text->sentences[j].str) + 1, sizeof(wchar_t));
                    wcscpy(copy_j, text->sentences[j].str);
                    wordToUpper(copy_j);
                    if (wcscmp(copy_i,copy_j) == 0) {
                        text->sentences[j].str = NULL;
                    }
                }
            }
        }
    }
}