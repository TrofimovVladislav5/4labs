#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <wchar.h>

typedef struct{
    wchar_t* str;
    int size;
    int wordCount;
    wchar_t** words;
    wchar_t** punctuations;
} Sentence;

typedef struct{
    Sentence* sentences;
    int size;
    int count;
} Text;

Sentence* createSentence();
Text* createText();

#endif