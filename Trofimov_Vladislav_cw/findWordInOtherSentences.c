#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include "structures.h"
#include "wordsToUpper.h"

void findWordInOtherSentences(Text* text) {
    if (text->sentences[0].wordCount >= 2 && text->sentences[0].str != NULL){
        wchar_t* copy = (wchar_t*) calloc(wcslen(text->sentences[0].words[1]) + 1, sizeof(wchar_t));
        wcscpy(copy,text->sentences[0].words[1]);
        wordToUpper(copy);
        for (int i = 0; i < text->count;i++){
            if (text->sentences[i].str != NULL) {
                for (int j = 0;j < text->sentences[i].wordCount;j++) {
                    wchar_t* copy_j = (wchar_t*) calloc(wcslen(text->sentences[i].words[j]) + 1, sizeof(wchar_t));
                    wcscpy(copy_j, text->sentences[i].words[j]);
                    wordToUpper(copy_j);
                    if (wcscmp(copy_j, copy) == 0) {
                        wprintf(L"\033[1;32m%ls\033[0m%ls", text->sentences[i].words[j],text->sentences[i].punctuations[j]);
                    }
                    else {
                        wprintf(L"%ls%ls", text->sentences[i].words[j],text->sentences[i].punctuations[j]);
                    }
                }
                wprintf(L"\n");
            }
        }
    }
}