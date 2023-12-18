#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "structures.h"

void splitSentence(Text* text) {
    wchar_t* sep = L" .,";
    wchar_t* copy;
    wchar_t* ptr;
    int len = 0;
    for (int i = 0; i < text->count; i++) {
        len = wcslen(text->sentences[i].str);
        wchar_t* copy = (wchar_t*) calloc(wcslen(text->sentences[i].str) + 1, sizeof(wchar_t));
        wcscpy(copy, text->sentences[i].str);
        text->sentences[i].words = (wchar_t**) malloc(text->sentences[i].wordCount * sizeof(wchar_t*));
        text->sentences[i].punctuations = (wchar_t**) malloc(text->sentences[i].wordCount * sizeof(wchar_t*));
        wchar_t* word = wcstok(copy, sep, &ptr);
        int j = 0;
        while (word != NULL) {
            wchar_t* wordCpy = (wchar_t*) calloc(wcslen(word)+1, sizeof(wchar_t));
            wcscpy(wordCpy, word);
            text->sentences[i].words[j] = wordCpy;
            j++;
            word = wcstok(NULL, sep, &ptr);
        }
        for (int k = 0; k < len; k++) {
            if (!wcschr(sep, text->sentences[i].str[k])) {
                text->sentences[i].str[k] = L'\0';
            }
        }
        j = 0;
        int none = 1;
        for (int k = 0; k < len; k++) {
            if (text->sentences[i].str[k] != L'\0' && none == 1) {
                text->sentences[i].punctuations[j] = &text->sentences[i].str[k];
                none = 0;
                j++;
            } else if (text->sentences[i].str[k] == L'\0') {
                none = 1;
            }
        }
    }
}