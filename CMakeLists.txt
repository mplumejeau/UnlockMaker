cmake_minimum_required(VERSION 3.24)
project(UnlockMaker C)

set(CMAKE_C_STANDARD 99)

######################################################################################

# Variable to chose the target to compile
# Write yes if you want to compile the unit tests target as a developer
# Write no if you want to compile the user target

set(UNIT_TESTS_MODE no)

######################################################################################

# Compilation options

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra")

# internal library path

set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# Model header files

set(MODEL_HEADERS
        Model/Struct.h
        Model/Project.h
        Model/algo.h
        Model/Card.h
        Model/Link.h
        Model/VertexList.h
        Model/EdgeList.h
        Model/Vertex.h
        Model/Edge.h)

# Model source files

set(MODEL_SOURCES
        Model/Project.c
        Model/algo.c
        Model/Card.c
        Model/Link.c
        Model/VertexList.c
        Model/EdgeList.c
        Model/Vertex.c
        Model/Edge.c)

# Model unit tests header files

set(UNIT_TESTS_HEADERS
        Model/TestsU/ProjectTestsU.h
        Model/TestsU/AlgoTestsU.h
        Model/TestsU/CardTestsU.h
        Model/TestsU/LinkTestsU.h
        Model/TestsU/VertexListTestsU.h
        Model/TestsU/EdgeListTestsU.h
        Model/TestsU/VertexTestsU.h
        Model/TestsU/EdgeTestsU.h)

# Model unit tests source files

set(UNIT_TESTS_SOURCES
        Model/TestsU/ProjectTestsU.c
        Model/TestsU/AlgoTestsU.c
        Model/TestsU/CardTestsU.c
        Model/TestsU/LinkTestsU.c
        Model/TestsU/VertexListTestsU.c
        Model/TestsU/EdgeListTestsU.c
        Model/TestsU/VertexTestsU.c
        Model/TestsU/EdgeTestsU.c)

# Model unit tests main

set(UNIT_TESTS_MAIN Model/TestsU/mainTestsU.c)

# View header files

set(VIEW_HEADERS
        View/ViewApplication.h
        View/GraphHandler.h)

# View source files

set(VIEW_SOURCES
        View/ViewApplication.c
        View/GraphHandler.c)

# Controller header files

set(CONTROLLER_HEADERS Controller/Controller.h)

# Controller source files

set(CONTROLLER_SOURCES Controller/Controller.c)

# Selection between unit tests mode or user mode

if(UNIT_TESTS_MODE STREQUAL yes)

    # Compilation options for generating information coverage about unit tests

    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

    # Unit tests executable

    add_executable(UnlockMakerTestsU
            ${MODEL_HEADERS} ${MODEL_SOURCES}
            ${UNIT_TESTS_HEADERS} ${UNIT_TESTS_SOURCES}
            ${UNIT_TESTS_MAIN})

    # Package CMocka required for unit tests

    find_package(cmocka REQUIRED)
    if(cmocka_FOUND)
        target_link_libraries(UnlockMakerTestsU cmocka)
    else()
        message(FATAL_ERROR "cmocka not found")
    endif()

    # Internal open source library libHaru for pdf generation

    include_directories(${LIB_DIR})
    target_link_libraries(UnlockMakerTestsU ${LIB_DIR}/libhpdfs.a)

    # Package zlib required for libHaru utilisation

    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIRS})
        target_link_libraries(UnlockMakerTestsU ${ZLIB_LIBRARIES})
    else()
        message(FATAL_ERROR "zlib not found")
    endif()

    # Maths library required for libHaru utilisation

    target_link_libraries(UnlockMakerTestsU m)

else()

    # User executable

    add_executable(UnlockMaker
            ${MODEL_HEADERS} ${MODEL_SOURCES}
            ${VIEW_HEADERS} ${VIEW_SOURCES}
            ${CONTROLLER_HEADERS} ${CONTROLLER_SOURCES})

    # Package PkgConfig required for checking libraries existence

    find_package(PkgConfig REQUIRED)
    if(NOT PkgConfig_FOUND)
        message(FATAL_ERROR "PkgConfig not found")
    endif()

    # Package GTK4 for the graphical interface of the software

    pkg_check_modules(GTK4 REQUIRED gtk4)
    if(GTK4_FOUND)
        target_include_directories(UnlockMaker PUBLIC ${GTK4_INCLUDE_DIRS})
        target_link_libraries(UnlockMaker ${GTK4_LIBRARIES})
        target_compile_options(UnlockMaker PUBLIC ${GTK4_CFLAGS_OTHER})
    else()
        message(FATAL_ERROR "gtk4 not found")
    endif()

    # Package libgvc for the graphical interface of the software

    pkg_check_modules(LIBGVC REQUIRED libgvc)
    if(GTK4_FOUND)
        target_include_directories(UnlockMaker PUBLIC ${LIBGVC_INCLUDE_DIRS})
        target_link_libraries(UnlockMaker ${LIBGVC_LIBRARIES})
        target_compile_options(UnlockMaker PUBLIC ${LIBGVC_CFLAGS_OTHER})
    else()
        message(FATAL_ERROR "libgvc not found")
    endif()

    # Package glib for the graphical interface of the software

    target_link_libraries(UnlockMaker ${GLIB_LIBRARIES})

    # Internal open source library libHaru for pdf generation

    include_directories(${LIB_DIR})
    target_link_libraries(UnlockMaker ${LIB_DIR}/libhpdfs.a)

    # Package zlib required for libHaru utilisation

    find_package(ZLIB REQUIRED)
    if(ZLIB_FOUND)
        include_directories(${ZLIB_INCLUDE_DIRS})
        target_link_libraries(UnlockMaker ${ZLIB_LIBRARIES})
    else()
        message(FATAL_ERROR "zlib not found")
    endif()

    # Maths library required for libHaru utilisation

    target_link_libraries(UnlockMaker m)

endif()



# Here are the commands to write in a terminal to create a .html file about the code coverage for the Model
# You have to be in the unlock-maker directory
# Then you will find the file index.html in the directory cmake-build-debug

# lcov --capture --directory cmake-build-debug/CMakeFiles/UnlockMakerTestsU.dir/Model --output-file coverage.info
# genhtml coverage.info --output-directory cmake-build-debug