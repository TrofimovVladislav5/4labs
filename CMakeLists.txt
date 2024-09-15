cmake_minimum_required(VERSION 3.29)
project(VladLox880)

set(CMAKE_CXX_STANDARD 14)

include_directories(ULL)

add_executable(VladLox880
    ULL/Node.cpp
    ULL/Node.h
    ULL/test.cpp
    ULL/test.h
    ULL/Unluck.cpp
    ULL/UnrolledLinkedList.cpp
    ULL/UnrolledLinkedList.h)
