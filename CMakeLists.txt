cmake_minimum_required(VERSION 3.23)
project(DA)

set(CMAKE_CXX_STANDARD 11)

add_executable(DA
        code/main.cpp code/graph.h code/graph.cpp code/supervisor.h code/supervisor.cpp code/menu.h
        code/menu.cpp code/station.h code/station.cpp code/vertexEdge.cpp code/vertexEdge.h)

