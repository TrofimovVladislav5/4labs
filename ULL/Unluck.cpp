#include "UnrolledLinkedList.h"

UnrolledLinkedList create_from_list() {
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

    return UnrolledLinkedList(b);
}

UnrolledLinkedList create_from_capacity(int capacity) {
    int count = 0;
    UnrolledLinkedList list = UnrolledLinkedList(capacity);
    std::string input;
    std::getline(std::cin,input);

    std::string tempValue;
    for (int i = 0;i < (int) input.size();i++){
        if (isdigit(input[i]) ||(input[i] == '-' && (i == 0 || input[i - 1] == ' '))){
            tempValue += input[i];
        }
        else if (input[i] == ' ' && !tempValue.empty()){
            int value = std::stoi(tempValue);
            list.insert(count, value);
            count++;
            tempValue.clear();
        }
    }
    if (!tempValue.empty()){
        int value = std::stoi(tempValue);
        list.insert(count, value);
        tempValue.clear();
    }

    return list;
}

int main(){
    UnrolledLinkedList list = create_from_capacity(10);
    list.pop(0);
    list.pop(6);
    list.pop(1);
    list.print();
}