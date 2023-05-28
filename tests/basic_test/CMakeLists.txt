add_executable(first_test basic_test.cpp)

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
list(APPEND EXTRA_LIBS serial lx16a_lib)

target_include_directories(first_test PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(first_test ${EXTRA_LIBS})