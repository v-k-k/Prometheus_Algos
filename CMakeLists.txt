cmake_minimum_required(VERSION 3.10.2)

project(Prometheus_Algos)

add_subdirectory(modules)
add_subdirectory(tests)
add_subdirectory(googletest)
include_directories(googletest/include)
add_executable(${PROJECT_NAME} main.c)
target_link_libraries(${PROJECT_NAME} optimized_math gtest)