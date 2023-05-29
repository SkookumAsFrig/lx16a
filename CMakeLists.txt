cmake_minimum_required(VERSION 3.10)

project(lx16a VERSION 0.2)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add_executable(${PROJECT_NAME} tests/hhm_test.cpp)

# uncomment following block for auto git pull latest branch
# commented out due to googletst c++14 run failure on newest version

# find_package(Git QUIET)
# if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
# # Update submodules as needed
#     option(GIT_SUBMODULE "Check submodules during build" ON)
#     if(GIT_SUBMODULE)
#         message(STATUS "Submodule update")
#         execute_process(COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
#                         WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
#                         RESULT_VARIABLE GIT_SUBMOD_RESULT)
#         if(NOT GIT_SUBMOD_RESULT EQUAL "0")
#             message(FATAL_ERROR "git submodule update --init --recursive failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
#         endif()
#     endif()
# endif()

# Update CMakeLists.txt path for more/different submodules

# if(NOT EXISTS "${PROJECT_SOURCE_DIR}/ext/myserial/CMakeLists.txt")
#     message(FATAL_ERROR "The submodules were not downloaded! GIT_SUBMODULE was turned off or failed. Please update submodules and try again.")
# endif()

add_subdirectory(${PROJECT_SOURCE_DIR}/src)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/hhm)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/poc)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/basic_test)
add_subdirectory(${PROJECT_SOURCE_DIR}/tests/unit)

add_subdirectory(${PROJECT_SOURCE_DIR}/ext/googletest)
add_subdirectory(${PROJECT_SOURCE_DIR}/ext/myserial)

# list(APPEND EXTRA_INCLUDES ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/ext/myserial/include ${PROJECT_SOURCE_DIR}/src ${PROJECT_SOURCE_DIR}/include)
# list(APPEND EXTRA_LIBS serial lx16a_lib)

# target_include_directories(${PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})

# target_link_libraries(${PROJECT_NAME} ${EXTRA_LIBS})