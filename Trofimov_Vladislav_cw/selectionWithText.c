#include <wchar.h>
#include <stdlib.h>
#include "cmp.h"
#include "structures.h"
#include "findWordInOtherSentences.h"
#include "output.h"
#include "deleteLongSentences.h"
#include "splitSentence.h"
#include "changeTense.h"

void selectionWithText(Text* text,int value){
    switch(value){
        case 0:
            output(text,value);
            break;
        case 1:
            splitSentence(text);
            changeTense(text);
            output(text,value);
            break;
        case 2:
            splitSentence(text);
            findWordInOtherSentences(text);
            break;
        case 3:
            qsort(text->sentences,text->count,sizeof(Sentence),cmp);
            output(text,value);
        case 4:
            deleteLongSentences(text);
            output(text,value);
            break;
    }
}