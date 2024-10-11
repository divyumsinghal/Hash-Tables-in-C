###
### CMake settings
###
# see http://www.cmake.org/Wiki/CMake_Policies

cmake_minimum_required(VERSION 3.15)

project(Practice)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


file(GLOB SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.C")

foreach(SRC_FILE ${SRC_FILES})

    get_filename_component(EXECUTABLE_NAME ${SRC_FILE} NAME_WE)
    
    add_executable(${EXECUTABLE_NAME} ${SRC_FILE})
endforeach()

file(COPY ${CMAKE_SOURCE_DIR}/data DESTINATION ${CMAKE_BINARY_DIR})
