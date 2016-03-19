project(5kgl)

cmake_minimum_required(VERSION 3.0.0)

find_package(OpenGL REQUIRED)
find_package(Threads REQUIRED)
find_package(X11 REQUIRED)

add_subdirectory(src)
