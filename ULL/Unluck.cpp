#include "UnrolledLinkedList.h"

int main(){
    std::string input;
    std::getline(std::cin,input);
    std::vector<int> b;
    std::string tempValue;
    for (int i = 0;i < (int) input.size();i++){
        if (isdigit(input[i]) ||(input[i] == '-' && (i == 0 || input[i - 1] == ' '))){
            tempValue += input[i];
        }
        else if (input[i] == ' ' && !tempValue.empty()){
            int value = std::stoi(tempValue);
            b.push_back(value);
            tempValue.clear();
        }
    }
    if (!tempValue.empty()){
        int value = std::stoi(tempValue);
        b.push_back(value);
        tempValue.clear();
    }
    UnrolledLinkedList list(b);
    list.pop(0);
    list.pop(0);
    list.print();
}