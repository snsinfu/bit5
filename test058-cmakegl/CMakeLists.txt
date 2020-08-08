cmake_minimum_required(VERSION 3.10)
project(test058)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(main main.c)
target_link_libraries(main OpenGL::GL glfw)

add_compile_definitions(GL_SILENCE_DEPRECATION)
