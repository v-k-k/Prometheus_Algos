cmake_minimum_required(VERSION 3.10.2)

project(Prometheus_Algos)

set(CMAKE_BUILD_TYPE Debug)

# Create the executable
add_executable(${PROJECT_NAME} main.c)

# Find CURL library
find_package(CURL REQUIRED)

# Find libzip library
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

# Include subdirectories
add_subdirectory(helpers)
add_subdirectory(modules)
add_subdirectory(tests)
add_subdirectory(googletest)

# Include directories
include_directories(${CURL_INCLUDE_DIRS})
include_directories(${LIBZIP_INCLUDE_DIRS})
include_directories(googletest/include)

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME} 
    simple_dynamic_string 
    tools 
    http
    payload_extractors
    zip_helper 
    optimized_math 
	sorting 
    ${CURL_LIBRARIES} 
    ${LIBZIP_LIBRARIES} 
    gtest)
