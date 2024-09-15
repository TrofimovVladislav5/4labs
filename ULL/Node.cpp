#include "Node.h"

#include <iostream>

Node::Node(int capacity){
    size = capacity;
    data = new int[size];
    current_node_size = 0;
    next = nullptr;
}

Node::~Node(){
    delete[] data;
}

int Node::getSize(){
    return size;
}

int Node::getCurrentNodeSize(){
    return current_node_size;
}

bool Node::nodeOverFlowed(){
    return current_node_size == size;
}

bool Node::nodeEmpty(){
    return size == 0;
}

int Node::getValueIndex(int index){
    if (index < 0 || index > current_node_size){
        throw std::out_of_range("Invalid Index getValueIndex");
    }
    return data[index];
}

void Node::insert(int index, int value){
    if (index < 0 || index > current_node_size){
        throw std::out_of_range("Invalid index insert");
    }

    if (nodeOverFlowed()){
        throw std::out_of_range("Node Overflowed");
    }

    for (int i = current_node_size;i > index;i--){
        data[i] = data[i - 1];
    }
    data[index] = value;
    current_node_size++;
}

void Node::pop(int index){
    if (index < 0 || index > current_node_size){
        throw std::out_of_range("Invalid index pop");
    }

    if (nodeEmpty()){
        throw std::out_of_range("Node is empty");
    }

    for (int i = index; i < current_node_size - 1;i++){
        data[i] = data[i + 1];
    }
    current_node_size--;
}

void Node::moveHalfToNext(Node* new_node){
    int half = (current_node_size + 1) / 2;
    for (int i = half;i < current_node_size;i++){
        new_node->pushBack(data[i]);
    }
    current_node_size = half;
}

void Node::mergeNodes(Node* new_node){
    for (int i = 0; i < new_node->getCurrentNodeSize();i++){
        insert(current_node_size,new_node->getValueIndex(i));
    }
    new_node = new_node->next;
}

void Node::moveValueToCurrentFromNext(Node* new_node){
    insert(current_node_size, new_node->getValueIndex(0));
    for (int i = 0; i < new_node->getCurrentNodeSize();i++){
        new_node->data[i] = new_node->data[i + 1];
    }
    new_node->current_node_size--;
}

void Node::pushBack(int value){
    insert(current_node_size, value);
}

void Node::pushFront(int value){
    insert(0, value);
}

void Node::popBack(){
    pop(current_node_size);
}

void Node::popFront(){
    pop(0);
}