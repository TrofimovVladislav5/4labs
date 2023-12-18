#include <stdlib.h>
#include <wchar.h>
#include <wctype.h>
#include <locale.h>
#include "structures.h"
#include "input_text.h"
#include "deleteDuplicates.h"
#include "courseWorkInfo.h"
#include "splitText.h"
#include "selectionWithText.h"
#include "checkPresentText.h"

int main(){
    setlocale(LC_CTYPE,"");
    courseWorkInfo();
    int command;
    getCommand(&command);
    if (checkPresentText(command)) {
        Text* text = createText();
        wchar_t* input_text = input(&(text->count));
        splitText(text,input_text);
        deleteDuplicates(text);
        selectionWithText(text,command);
    }
}