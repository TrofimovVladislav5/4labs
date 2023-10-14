#include <stdio.h>

#define N 100

int input_arr(int arr[]){
    int count = 0;
    char space = ' ';
    while (count < N && space == ' '){
        scanf("%d%c", &arr[count],&space);
        count++;
    }
    return count;
}