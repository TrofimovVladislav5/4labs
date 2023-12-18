#include "structures.h"

void deleteLongSentences(Text* text) {
    for (int i = 0; i < text->count; i++) {
        if (text->sentences[i].wordCount > 10) {
            text->sentences[i].str = NULL;
        }
    }
}