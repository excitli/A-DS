cmake_minimum_required(VERSION 3.10)

project(MST_Graph_Solver)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)


add_executable(app 
    mst.cpp 
    timsort.h 
    intarray.h
)