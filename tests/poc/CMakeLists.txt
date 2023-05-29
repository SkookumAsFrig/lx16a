set(EXEC_NAME poc)

# message(STATUS "exec name of first_test is ${EXEC_NAME}, cmake src dir is ${CMAKE_SOURCE_DIR}")

add_executable(${EXEC_NAME} proof_of_concept.cpp)

# debug message
# message(STATUS "I am here: project name ${PROJECT_NAME}, project source dir ${PROJECT_SOURCE_DIR}")

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
list(APPEND EXTRA_LIBS serial lx16a_lib)

target_include_directories(${EXEC_NAME} PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(${EXEC_NAME} ${EXTRA_LIBS})