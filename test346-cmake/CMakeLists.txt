cmake_minimum_required(VERSION 3.10)
project(test346)

find_package(test346 REQUIRED HINTS "cmake")

OPTION(TEST346_BUILD_EXAMPLE "build example" OFF)

if(TEST346_BUILD_EXAMPLE)
    add_subdirectory(example)
endif()
