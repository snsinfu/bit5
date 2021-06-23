cmake_minimum_required(VERSION 3.10)
project(test348)


# Load package from test346. A package (with ${name}) must provide a config file
# named ${name}-config.cmake in a cmake prefix path. The config file defines
# libraries by calling add_library. This way, one can use a cmake-based header-
# only library without installing it to the system.
#
# A good cmake-based codebase to read:  https://github.com/seqan/seqan3
list(APPEND CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../test346-cmake/cmake")
find_package(test346 REQUIRED)


# Then, usual cmake directives for building a program follow.
set(PRODUCT main)
set(SOURCE_FILES main.cpp)

add_executable(${PRODUCT} ${SOURCE_FILES})

set_property(
    TARGET   ${PRODUCT}
    PROPERTY CXX_STANDARD          17
             CXX_STANDARD_REQUIRED ON
             CXX_EXTENSIONS        OFF
)

# cmake --trace-expand confirms that add_library and target_include_directories
# from test346 is invoked.
target_link_libraries(${PRODUCT} PRIVATE test346)
