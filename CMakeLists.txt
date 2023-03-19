cmake_minimum_required(VERSION 3.24)
project(UnlockMaker C)

set(CMAKE_C_STANDARD 90)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")
add_executable(UnlockMaker Model/main.c Model/Card.c Model/Card.h Model/Edge.h Model/EdgeList.h Model/Link.h
        Model/Project.h Model/Vertex.h Model/VertexList.h Model/Vertex.c Model/VertexList.c Model/VertexListTestsU.c Model/Edge.c Model/EdgeList.c)

target_link_libraries(UnlockMaker ${GLIB_LIBRARIES})
target_link_libraries(UnlockMaker ${GTK4_LIBRARIES})
target_include_directories(UnlockMaker PUBLIC ${GTK4_INCLUDE_DIRS})
target_compile_options(UnlockMaker PUBLIC ${GTK4_CFLAGS_OTHER})