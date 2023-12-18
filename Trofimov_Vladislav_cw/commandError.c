#include <wchar.h>
#include <stdlib.h>

void commandError(){
    wprintf(L"\033[1;31mError: Введённая команда не существует. Для ознакомления с командами введите 5.\033[0m\n");
}