add_executable(poc proof_of_concept.cpp)

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
list(APPEND EXTRA_LIBS serial lx16a_lib)

target_include_directories(poc PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(poc ${EXTRA_LIBS})