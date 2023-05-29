set(EXEC_NAME unit)

# message(STATUS "exec name of first_test is ${EXEC_NAME}, cmake src dir is ${CMAKE_SOURCE_DIR}")

add_executable(${EXEC_NAME} unit_test.cpp)

enable_testing()

# message(STATUS "binary dir is ${CMAKE_BINARY_DIR}")

# debug message
# message(STATUS "I am here: project name ${PROJECT_NAME}, project source dir ${PROJECT_SOURCE_DIR}")

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/ext/googletest/googletest/include
 ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

list(APPEND EXTRA_LIBS serial lx16a_lib gtest gtest_main)

target_include_directories(${EXEC_NAME} PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(${EXEC_NAME} ${EXTRA_LIBS})


include(GoogleTest)

gtest_discover_tests(${EXEC_NAME})