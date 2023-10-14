#include <stdio.h>

#include "max.h"
#include "min.h"
#include "diff.h"
#include "sum.h"

#define format "%d\n"

int output(int command, int count, int *arr) {
    switch(command) {
        case 0:
            printf(format, arr[max(arr,count)]);
            break;
        case 1:
            printf(format, arr[min(arr, count)]);
            break;
        case 2:
            printf(format, diff(arr,count));
            break;
        case 3:
            printf(format, sum(arr,count));
            break;
        default:
            printf("Данные некорректны");
            break;
    }
}