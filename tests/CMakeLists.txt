cmake_minimum_required(VERSION 3.10.2)

project(tests)
add_executable(${PROJECT_NAME} test_karatsuba.cpp test_inversions_counting.cpp)
set(CMAKE_CXX_STANDARD 11)
target_link_libraries(${PROJECT_NAME} PUBLIC 
    tools 
    optimized_math 
    sorting 
    zip_helper 
    simple_dynamic_string 
    payload_extractors
    ${CURL_LIBRARIES} 
    ${LIBZIP_LIBRARIES}
    gtest
    gtest_main
)
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