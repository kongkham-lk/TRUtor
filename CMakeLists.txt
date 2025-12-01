# If fail to compile, Run command:
#   cmake -S . -B cmake-build-debug
#   cmake --build cmake-build-debug

cmake_minimum_required(VERSION 4.0)
project(TRUtor)

set(CMAKE_CXX_STANDARD 20)


# Auto-detect all .cpp and .h files recursively
file(GLOB_RECURSE SOURCES
        ${PROJECT_SOURCE_DIR}/*.cpp
        ${PROJECT_SOURCE_DIR}/model/*.cpp
        ${PROJECT_SOURCE_DIR}/service/*.cpp
        ${PROJECT_SOURCE_DIR}/controller/*.cpp
)


# Remove generated CMake files from SOURCES
foreach(FILE ${SOURCES})
    if(FILE MATCHES ".*CMakeFiles/.*")
        list(REMOVE_ITEM SOURCES ${FILE})
    endif()
endforeach()

add_executable(TRUtor ${SOURCES})