cmake_minimum_required(VERSION 3.10.2)

project(tests)
add_executable(${PROJECT_NAME} test_karatsuba.cpp test_inversions_counting.cpp)#

set(CMAKE_CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME} PUBLIC 
    simple_dynamic_string
    tools 
    http
    payload_extractors
    optimized_math 
    sorting
    zip_helper  
    ${CURL_LIBRARIES} 
    ${LIBZIP_LIBRARIES}
    gtest
    gtest_main
)

# find_package(CURL REQUIRED)

# find_package(PkgConfig REQUIRED)
# pkg_check_modules(LIBZIP REQUIRED libzip)

# add_subdirectory(helpers)
#add_subdirectory(modules)
#add_subdirectory(tests)
#add_subdirectory(googletest)

# include_directories(${CURL_INCLUDE_DIRS})
#include_directories(${LIBZIP_INCLUDE_DIRS})
#include_directories(googletest/include)
#############################################
#target_link_libraries(${PROJECT_NAME} 
#tools 
#optimized_math 
#sorting 
#zip_helper 
#simple_dynamic_string 
#payload_extractors
#${CURL_LIBRARIES} 
#${LIBZIP_LIBRARIES} 
#gtest)