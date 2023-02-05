cmake_minimum_required(VERSION 3.5)

project(timed_task LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(timed_task main.cpp timed_task.cpp timed_task.h)

install(TARGETS timed_task
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})
