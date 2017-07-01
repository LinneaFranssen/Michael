cmake_minimum_required(VERSION 3.6)
project(lf_bugs)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        .gitignore
        cell.cpp
        cell.h
        grid.cpp
        grid.h
        main.cpp
        main.h
        README
        wasp.cpp
        wasp.h
)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(lf_bugs ${SOURCE_FILES})