cmake_minimum_required(VERSION 3.10)
project(test346::example)

set(PRODUCT main)
set(SOURCE_FILES main.cpp)
set(GCC_WARNINGS "-Wpedantic -Wall -Wextra -Wconversion -Wsign-conversion")

add_executable(${PRODUCT} ${SOURCE_FILES})

set_property(
    TARGET   ${PRODUCT}
    PROPERTY CXX_STANDARD          17
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS        OFF
)

# FIXME
# target_link_libraries(${PRODUCT} INTERFACE test346)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_WARNINGS}")
endif()
