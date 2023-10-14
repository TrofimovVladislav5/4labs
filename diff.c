#include "max.h"
#include "min.h"

int diff(int arr[], int count){
    return arr[max(arr,count)] - arr[min(arr,count)];
}