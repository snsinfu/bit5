cmake_minimum_required(VERSION 3.10)
project(test346)

add_library(${PROJECT_NAME} INTERFACE)
target_include_directories(${PROJECT_NAME} INTERFACE include)

OPTION(TEST346_BUILD_EXAMPLE "build example" OFF)

if(TEST346_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
