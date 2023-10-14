int max(int arr[],int count){
    int max_index = 0;
    for (int i = 1; i < count; i++) {
        if (arr[i] > arr[max_index]) {
            max_index = i;
        }
    }
    return max_index;
}
