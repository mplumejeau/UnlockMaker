cmake_minimum_required(VERSION 3.24)
project(UnlockMaker C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

# CONTROLLER EXECUTABLE

add_executable(UnlockControl
        Controller/Controller.c Controller/Controller.h
        View/ViewApplication.c View/ViewApplication.h
        View/GraphHandler.h View/GraphHandler.c
        Model/Project.h Model/Project.c
        Model/algo.h Model/algo.c
        Model/Vertex.h Model/Vertex.c
        Model/VertexList.h Model/VertexList.c
        Model/Card.h Model/Card.c
        Model/Edge.h Model/Edge.c
        Model/EdgeList.h Model/EdgeList.c
        Model/Link.h Model/Link.c)

# MAIN EXECUTABLE

# à dé-commenter pour utiliser gtk et gvc

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED gtk4)
pkg_check_modules(LIBGVC REQUIRED libgvc)

add_executable(UnlockMaker
        Model/main.c Model/Struct.h
        Model/Project.h Model/Project.c
        Model/algo.h Model/algo.c
        Model/Vertex.h Model/Vertex.c
        Model/VertexList.h Model/VertexList.c
        Model/Card.h Model/Card.c
        Model/Edge.h Model/Edge.c
        Model/EdgeList.h Model/EdgeList.c
        Model/Link.h Model/Link.c)

target_link_libraries(UnlockControl ${GLIB_LIBRARIES})
target_link_libraries(UnlockControl ${GTK4_LIBRARIES})
target_include_directories(UnlockControl PUBLIC ${GTK4_INCLUDE_DIRS})
target_compile_options(UnlockControl PUBLIC ${GTK4_CFLAGS_OTHER})
target_link_libraries(UnlockControl ${LIBGVC_LIBRARIES})
target_include_directories(UnlockControl PUBLIC ${LIBGVC_INCLUDE_DIRS})
target_compile_options(UnlockControl PUBLIC ${LIBGVC_CFLAGS_OTHER})

# UNIT TESTS EXECUTABLE

find_package(cmocka REQUIRED)

add_executable(UnlockMakerTestsU
        Model/TestsU/mainTestsU.c Model/Struct.h
        Model/Vertex.h Model/Vertex.c Model/TestsU/VertexTestsU.h Model/TestsU/VertexTestsU.c
        Model/VertexList.h Model/VertexList.c Model/TestsU/VertexListTestsU.h Model/TestsU/VertexListTestsU.c
        Model/Card.h Model/Card.c
        Model/Edge.h Model/Edge.c Model/TestsU/EdgeTestsU.h Model/TestsU/EdgeTestsU.c
        Model/EdgeList.h Model/EdgeList.c Model/TestsU/EdgeListTestsU.h Model/TestsU/EdgeListTestsU.c
        Model/Link.h Model/Link.c Model/TestsU/LinkTestsU.h Model/TestsU/LinkTestsU.c
        Model/Project.h Model/Project.c
        Model/algo.h Model/algo.c)

target_link_libraries(UnlockMakerTestsU cmocka)

add_custom_target(coverage
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_BINARY_DIR}/coverage
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage cmake -E remove *
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage cmake -E env CTEST_OUTPUT_ON_FAILURE=1 ${CMAKE_CTEST_COMMAND} -j4
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage lcov --directory . --capture --output-file coverage.info
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage lcov --remove coverage.info '/usr/*' "${PROJECT_SOURCE_DIR}/tests/*" --output-file coverage.info.cleaned
        COMMAND ${CMAKE_COMMAND} -E chdir ${CMAKE_BINARY_DIR}/coverage genhtml --output-directory html coverage.info.cleaned
        DEPENDS UnlockMakerTestsU)

add_dependencies(${PROJECT_NAME} coverage)

# Here are the commands to write in a terminal to create a .html file about the code coverage for the Model
# You have to be in the unlock-maker directory
# Then you will find the file index.html in the directory cmake-build-debug

# lcov --capture --directory cmake-build-debug/CMakeFiles/UnlockMakerTestsU.dir/Model --output-file coverage.info
# genhtml coverage.info --output-directory cmake-build-debug
