#include <stdlib.h>
#include <wchar.h>

void getCommand(int* command){
    wscanf(L"%d", command);
}

wchar_t* input(int* count){
    int size = 0;
    int capacity = 1;
    int flag = 0;
    wchar_t current_wch = getwchar();
    wchar_t* buf = (wchar_t*) malloc(sizeof(wchar_t));
    while(flag < 2){
        current_wch = getwchar();
        if (current_wch == L'\n') flag++;
        else flag = 0;
        if (size + 1 >= capacity){
            capacity *= 2;
            wchar_t* new_buf = (wchar_t*) realloc(buf,capacity * sizeof(wchar_t));
            buf = new_buf;
        }
        if (current_wch == L'.'){
            (*count)++;
        }
        buf[size++] = current_wch;
    }
    buf[size - 2] = L'\0';
    return buf;
}