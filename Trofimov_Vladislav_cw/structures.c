#include <wchar.h>
#include <stdlib.h>
#include "structures.h"

Sentence* createSentence(){
    Sentence* sentence = (Sentence*) malloc(sizeof(Sentence));
    sentence->str = NULL;
    sentence->size = 0;
    return sentence;
}

Text* createText(){
    Text* text = (Text*) malloc(sizeof(Text));
    text->sentences = (Sentence*) malloc(sizeof(Sentence));
    text->size = 0;
    text->count = 0;
    return text;
}