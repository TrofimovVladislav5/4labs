#include <stdio.h>

#include "max.h"
#include "min.h"
#include "diff.h"
#include "sum.h"
#include "output.h"
#include "input_command.h"
#include "input_arr.h"

#define N 100

int main(){
    int command = input_command();
    int arr[N];
    int count = input_arr(arr);
    output(command,count,arr);
}