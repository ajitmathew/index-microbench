cmake_minimum_required(VERSION 3.12)
project(LinkedList)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(LinkedList
        list.h
        listNode.cpp
        listNode.h)
