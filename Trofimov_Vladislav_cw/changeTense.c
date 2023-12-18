#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include "structures.h"
#include "wordsToUpper.h"

void replaceEnding(wchar_t* word){
    int wordLen = wcslen(word);
    wchar_t* copy = (wchar_t*) calloc(wcslen(word) + 1, sizeof(wchar_t));
    wcscpy(copy,word);
    wordToUpper(copy);
    if (wordLen >= 4) {
        if (wcscmp(&copy[wordLen - 4], L"ТЬСЯ") == 0){
            wcscpy(&word[wordLen - 4], L"тся");
            return;
        }
    }
    if (wordLen >= 3) {
        if (wcscmp(&copy[wordLen - 3], L"ТСЯ") == 0){
            word = (wchar_t*) realloc(word, wordLen+2);
            wcscpy(&word[wordLen - 3], L"ться");
            word[wordLen + 1] = L'\0';
        }
    }
}

void changeTense(Text* text){
    for (int i = 0;i < text->count;i++) {
        if (text->sentences[i].str != NULL){
            for(int k = 0;k < text->sentences[i].wordCount;k++){
                replaceEnding(text->sentences[i].words[k]);
            }
        }
    }
}