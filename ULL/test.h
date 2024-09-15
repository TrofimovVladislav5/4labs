#include "UnrolledLinkedList.h"
#include <cassert>
#ifndef TEST_H
#define TEST_H
void check(std::vector<int>& arrayToAdd, std::vector<int>& arrToDelete);
void checkPushFront(std::vector<int>& array, int value);
void checkPushBack(std::vector<int>& array, int value);
void checkInsert(std::vector<int>& array, int index, int value);
#endif