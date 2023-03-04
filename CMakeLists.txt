cmake_minimum_required(VERSION 3.24)
project(UnlockMaker C)

set(CMAKE_C_STANDARD 90)

add_executable(UnlockMaker main.c Card.c Car.h Edge.h EdgeList.h Link.h Project.h Vertex.h VertexList.h Model/Vertex.c)
