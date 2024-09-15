#include <cassert>
#include "UnrolledLinkedList.h"
#include "test.h"

void check(std::vector<int>& arrayToAdd, std::vector<int>& arrToDelete){
    UnrolledLinkedList list((int) arrayToAdd.size());
    for (int i = 0;i < (int) arrayToAdd.size();i++){
        list.insert(i, arrayToAdd[i]);
    }
    std::cout << "List " << std::endl;
    list.print();
    std::cout << std::endl << "Elements for removing: ";
    for (int elem : arrToDelete){
        std::cout << elem << " ";
    }
    std::cout << std::endl;

    for (int i = 0; i <  (int) arrayToAdd.size();i++){
        int index = list.find(arrayToAdd[i]);
        assert(index == i && "Element not found");
    }

    for (int i = 0;i < (int) arrToDelete.size();i++){
        int index = list.find(arrToDelete[i]);
        if (index != -1){
            std::cout << "Element " << arrToDelete[i] << " Index element " << index << " pop " << std::endl;
            list.pop(index);
            list.print();
            std::cout << std::endl;
        }
        else {
            std::cout << "Element " << arrToDelete[i] << " not found " << std::endl;
        }

        int new_index = list.find(arrToDelete[i]);
        assert(new_index == -1);
    }

    std::cout << "Check Successfully" << std::endl;
}

void checkPushFront(std::vector<int>& array, int value){
    std::vector<int> expectedList;
    for (int elem : array){
        expectedList.push_back(elem);
    }
    expectedList.insert(expectedList.begin(), value);
    UnrolledLinkedList list(array);
    list.insert(0, value);
    for (int i = 0;i < (int) expectedList.size();i++){
        int index = list.find(expectedList[i]);
        assert(index == i);
    }
    std::cout << "pushFront Succesfully" << std::endl;
}

void checkPushBack(std::vector<int>& array, int value){
    std::vector<int> expectedList;
    for (int elem : array){
        expectedList.push_back(elem);
    }
    expectedList.push_back(value);
    UnrolledLinkedList list(array);
    list.insert(array.size(), value);
    for (int i = 0;i < (int) expectedList.size();i++){
        int index = list.find(expectedList[i]);
        assert(index == i);
    }
    std::cout << "pushBack Succesfully" << std::endl;
}

void checkInsert(std::vector<int>& array, int index, int value){
    UnrolledLinkedList list(array);
    std::vector<int> expectedList = array;
    expectedList.insert(expectedList.begin() + index, value);
    list.insert(index, value);

    for (int i = 0;i < (int) expectedList.size();i++){
        int index = list.find(expectedList[i]);
        assert(index == i);
    }
    std::cout << "Insert Successfully" << std::endl;
}

std::vector<int> createList(int countElements){
    std::vector<int> list;
    for (int i = 0;i < countElements;i++){
        list.push_back(i);
    }
    return list;
}

// int main(){
//     std::vector<int> elements = createList(100);
//     std::vector<int> removingElenents = createList(10);
//     check(elements, removingElenents);
//     checkPushFront(elements, 101);
//     checkPushBack(elements, 101);
//     checkInsert(elements, 25, 101);
// }