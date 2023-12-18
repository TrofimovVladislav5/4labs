#include <wchar.h>
#include <stdlib.h>

int getWords(wchar_t sentence[]) {
    int wordCount = 0;
    wchar_t sep[] = L" .,";
    for (int i = 0; i < wcslen(sentence); i++) {
        if (!wcschr(sep, sentence[i]) && (wcschr(sep, sentence[i + 1]) || sentence[i + 1] == L'\0')) {
            wordCount++;
        }
    }
    return wordCount;
}