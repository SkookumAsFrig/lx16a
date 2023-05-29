add_library(lx16a_lib lx16a.cpp)

list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)

target_include_directories(lx16a_lib PUBLIC ${EXTRA_INCLUDES})

target_link_libraries(lx16a_lib serial)
