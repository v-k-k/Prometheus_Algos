add_executable("runUnitTests" test_karatsuba.cpp test_inversions_counting.cpp test_sorting.cpp)

target_link_libraries("runUnitTests" PUBLIC 
    simple_dynamic_string
    tools 
    http
    payload_extractors
    optimized_math 
    sorting
    zip_helper  
    sqlite_client
    m
    ${SQLITE3_LIBRARIES}
    ${CURL_LIBRARIES} 
    ${LIBZIP_LIBRARIES}
    gtest
    gtest_main
    gmock_main)
