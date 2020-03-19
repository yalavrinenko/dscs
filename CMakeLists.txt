cmake_minimum_required(VERSION 3.10)

project(dscs)

set(CMAKE_CXX_STANDARD 17)

file(GLOB_RECURSE SRC src/*.cpp)
file(GLOB_RECURSE HEADERS src/*.hpp)

message(STATUS "[SRC] Found files: ${SRC} ${HEADERS}")

add_library(logger utils/logger_factory.cpp utils/logger_factory.hpp)
target_link_libraries(logger stdc++fs)

add_subdirectory(tests)

add_library(dscs-lib ${SRC} ${HEADERS})
add_executable(dscs entry.cpp)

target_link_libraries(dscs stdc++fs pthread logger dscs-lib)