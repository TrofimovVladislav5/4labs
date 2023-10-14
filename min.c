int min(int arr[],int count){
    int min_index = 0;
    for(int i = 1; i < count; i++) {
        if (arr[i] < arr[min_index]){
            min_index = i;
        }
    }
    return min_index;
}