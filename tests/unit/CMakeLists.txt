add_executable(unit unit_test.cpp)

enable_testing()

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} 
${PROJECT_SOURCE_DIR}/ext/serial/include 
${PROJECT_SOURCE_DIR}/ext/googletest/googletest/include
${PROJECT_SOURCE_DIR}/src 
${PROJECT_SOURCE_DIR}/include)
list(APPEND EXTRA_LIBS serial lx16a_lib gtest)

target_include_directories(unit PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(unit ${EXTRA_LIBS})

include(GoogleTest)
gtest_discover_tests(unit)
