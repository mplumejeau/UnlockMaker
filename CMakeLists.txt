cmake_minimum_required(VERSION 3.24)
project(UnlockMaker C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -pedantic-errors")

# MAIN EXECUTABLE

# a decommenter pour utiliser gtk
#
#find_package(PkgConfig REQUIRED)
#pkg_check_modules(GTK4 REQUIRED gtk4)
#

add_executable(UnlockMaker
        Model/main.c Model/Struct.h
        Model/Project.h
        Model/Vertex.h Model/Vertex.c
        Model/VertexList.h Model/VertexList.c
        Model/Card.h Model/Card.c
        Model/Edge.h Model/Edge.c
        Model/EdgeList.h Model/EdgeList.c
        Model/Link.h)

target_link_libraries(UnlockMaker ${GLIB_LIBRARIES})
target_link_libraries(UnlockMaker ${GTK4_LIBRARIES})
target_include_directories(UnlockMaker PUBLIC ${GTK4_INCLUDE_DIRS})
target_compile_options(UnlockMaker PUBLIC ${GTK4_CFLAGS_OTHER})

# UNIT TESTS EXECUTABLE

find_package(cmocka REQUIRED)

add_executable(UnlockMakerTestsU
        Model/TestsU/mainTestsU.c Model/Struct.h
        Model/Vertex.h Model/Vertex.c Model/TestsU/VertexTestsU.h Model/TestsU/VertexTestsU.c
        Model/VertexList.h Model/VertexList.c Model/TestsU/VertexListTestsU.h Model/TestsU/VertexListTestsU.c
        Model/Card.h Model/Card.c)

target_link_libraries(UnlockMakerTestsU cmocka)