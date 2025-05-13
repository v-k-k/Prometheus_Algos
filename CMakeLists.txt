cmake_minimum_required(VERSION 3.10.2)

project(
    "Prometheus_Algos"
    VERSION 1.0.0
    LANGUAGES C CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(CMAKE_BUILD_TYPE Debug)

# Options
option(USE_CPM "Whether to use CPM." ON)
option(ENABLE_TESTING "Enable a Unit Testing build." ON)

# CMAKE MODULES
list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(AddGitSubmodule)

# Find CURL library
find_package(CURL REQUIRED)

# Find libzip library
find_package(PkgConfig REQUIRED)
pkg_check_modules(LIBZIP REQUIRED libzip)

# Find SQLite3 library
pkg_search_module(SQLITE3 REQUIRED sqlite3)
#find_package(SQLite3 REQUIRED)

# Create the executable
add_executable(${PROJECT_NAME} main.c)

# EXTERNAL LIBRARIES
if(USE_CPM)
    message(STATUS "Using CPM")
    include(CPM)
	cpmaddpackage("gh:google/googletest#v1.15.2")
endif()

# Include subdirectories
add_subdirectory(helpers)
add_subdirectory(modules)

if(ENABLE_TESTING)
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
	add_test(NAME AlgorithmsTests COMMAND runUnitTests)
endif()

# Link the necessary libraries
target_link_libraries(${PROJECT_NAME} 
    simple_dynamic_string 
    tools 
    http
    payload_extractors
    zip_helper 
    optimized_math 
    sorting 
    sqlite_client
    ${SQLITE3_LIBRARIES}
    ${CURL_LIBRARIES} 
    ${LIBZIP_LIBRARIES})
