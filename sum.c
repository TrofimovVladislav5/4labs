#include "min.h"

int sum(int arr[],int count){
    int sum = 0;
    int min_index = min(arr,count);
    for(int i = 0; i < min_index;i++){
        sum += arr[i];
    }
    return sum;
}