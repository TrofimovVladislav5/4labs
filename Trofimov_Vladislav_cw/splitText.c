#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include "structures.h"
#include "input_text.h"
#include "getWords.h"

void splitText(Text* text, wchar_t* input){
    text->sentences = (Sentence*) malloc(text->count * sizeof(Sentence));
    wchar_t* ptr;
    wchar_t* sentence = wcstok(input,L".", &ptr);
    int j = 0;
    while (sentence != NULL){
        while (iswspace(*sentence)){
            sentence++;
        }
        text->sentences[j].str = (wchar_t*)calloc(wcslen(sentence) + 2, sizeof(wchar_t));
        wcscpy(text->sentences[j].str, sentence);
        text->sentences[j].str[wcslen(sentence)] = L'.';
        text->sentences[j].size = wcslen(text->sentences[j].str);
        text->sentences[j].wordCount = getWords(sentence);
        j++;
        sentence = wcstok(NULL, L".", &ptr);
    }
}