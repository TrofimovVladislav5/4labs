#include "UnrolledLinkedList.h"

UnrolledLinkedList::UnrolledLinkedList(int capacity){
    size = calculateOptimalNodeSize(capacity);
    current_size = capacity;
    head = new Node(size);
}

UnrolledLinkedList::~UnrolledLinkedList() = default;

UnrolledLinkedList::UnrolledLinkedList(std::vector<int>& list){
    size = calculateOptimalNodeSize(list.size());
    current_size = static_cast<int>(list.size());

    if (size == 0) head = nullptr;
    else head = new Node(size);

    Node* current = head;
    for (int i = 0;i < (int) list.size();i++){
        if (current->getCurrentNodeSize() == size / 2){
            Node* new_node = new Node(size);
            current->next = new_node;
            current = new_node;
        }
        current->insert(current->getCurrentNodeSize(), list[i]);
    }
}

void UnrolledLinkedList::insert(int index, int value){
    if (index < 0 || index > current_size){
        throw std::out_of_range("Invalid Index Error 2");
    }
    Node* current = head;
    while (current != nullptr && index > current->getCurrentNodeSize()){
        index -= current->getCurrentNodeSize();
        current = current->next;
    }

    if (current->nodeOverFlowed()){
        Node* new_node = new Node(size);
        current->moveHalfToNext(new_node);
        new_node->next = current->next;
        current->next = new_node;
        if (index > current->getCurrentNodeSize()){
            index -= current->getCurrentNodeSize();
            current = new_node;
        }
    }
    current->insert(index, value);
    current_size++;
}

void UnrolledLinkedList::pop(int index){
    if (index < 0 || index > current_size){
        throw std::out_of_range("Invalid Index Error");
    }

    Node* current = head;
    while (current != nullptr && index > current->getCurrentNodeSize()){
        index -= current->getCurrentNodeSize();
        current = current->next;
    }

    current->pop(index);
    if (current->getCurrentNodeSize() < size / 2){
        if (current->next) {
            if (current->next->getCurrentNodeSize() <= size / 2){
                current->mergeNodes(current->next);
                Node* temp = current->next;
                current->next = current->next->next ? current->next->next : nullptr;
                delete temp;
            }
            else {
                current->moveValueToCurrentFromNext(current->next);
            }
        }
    }
    current_size--;
    if (current_size == 0){
        head = nullptr;
    }
}

int UnrolledLinkedList::find(int value){
    Node* current = head;
    int index = 0;
    while (current){
        for (int i = 0; i < current->getCurrentNodeSize();i++){
            if (current->getValueIndex(i) == value){
                return index;
            }
            index++;
        }
        current = current->next;
    }
    return -1;
}

void UnrolledLinkedList::popFront(){
    pop(0);
}

void UnrolledLinkedList::popBack(){
    pop(current_size - 1);
}

void UnrolledLinkedList::print(){
    Node* current = head;
    int count = 0;
    while(current){
        std::string result;
        for(int i = 0;i < current->getCurrentNodeSize();i++){
            result += std::to_string(current->getValueIndex(i)) + " ";
        }
        result.pop_back();
        std:: cout << "Node " + std::to_string(count) << ": " << result << std::endl;
        count++;
        current = current->next;
    }
}

int UnrolledLinkedList::calculateOptimalNodeSize(int num_elements){
    int minimal_cache_line_size = 64;
    float memory_num_elements = sizeof(int) * num_elements;
    float memory_buffer = std::ceil(memory_num_elements / minimal_cache_line_size);
    int optimal_size = (memory_buffer + 1) * 2;
    return optimal_size;
}
