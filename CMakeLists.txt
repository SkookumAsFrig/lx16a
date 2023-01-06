cmake_minimum_required(VERSION 3.10)

project(lx16a VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(${PROJECT_NAME} tests/test.cpp)

add_subdirectory(ext/serial)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_BINARY_DIR}
    PUBLIC ext/serial/include
)

target_link_libraries(${PROJECT_NAME} serial)