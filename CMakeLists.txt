cmake_minimum_required(VERSION 3.10)
project(SIR VERSION 1.0)

FIND_PACKAGE(SFML 2.5 COMPONENTS graphics REQUIRED)

INCLUDE_DIRECTORIES("${PROJECT_SOURCE_DIR}/Parte1" "${PROJECT_SOURCE_DIR}/Parte2")

ADD_SUBDIRECTORY(Parte1)
ADD_SUBDIRECTORY(Parte2)

SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra -fsanitize=address")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")