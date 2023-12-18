#include <wchar.h>
#include "structures.h"

void output(Text* text, int value){
    if (value == 1){
        for (int i = 0; i < text->count;i++){
            for (int k = 0; k < text->sentences[i].wordCount;k++){
                wprintf(L"%ls%ls", text->sentences[i].words[k], text->sentences[i].punctuations[k]);
            }
            wprintf(L"\n");
        }
    }
    else{
        for (int i = 0; i < text->count; i++){
            if (text->sentences[i].str != NULL){
                wprintf(L"%ls\n", text->sentences[i].str);
            }
        }
    }
}