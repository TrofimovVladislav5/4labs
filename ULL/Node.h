#pragma once
#include <stdexcept>

class Node {
public:
    Node* next;
    Node(int capacity);
    ~Node();
    int getSize();
    int getCurrentNodeSize();
    int getValueIndex(int index);

    bool nodeOverFlowed();
    bool nodeEmpty();

    void insert(int index, int value);
    void pop(int index);

    void moveHalfToNext(Node* new_node);
    void mergeNodes(Node* new_node);
    void moveValueToCurrentFromNext(Node* new_node);

    void pushBack(int value);
    void pushFront(int value);
    void popBack();
    void popFront();
    int current_node_size;
private:
    int size;
    int* data;
};