cmake_minimum_required(VERSION 3.10)

set(TARGET_BIN "game")
set(SD2_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/SDL)
set(SD2_IMAGE_LIB_DIR ${CMAKE_SOURCE_DIR}/lib/SDL2_image-2.6.3)

project(${TARGET_BIN})

set(CMAKE_C_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/build)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(include)
include_directories(${SD2_LIB_DIR}/include)
include_directories(${SD2_IMAGE_LIB_DIR})

add_subdirectory(${SD2_LIB_DIR})
add_subdirectory(${SD2_IMAGE_LIB_DIR})

file(GLOB SOURCES src/*.* src/*/*.*)

find_package(SDL2 REQUIRED)

add_executable(${TARGET_BIN} ${SOURCES})
target_link_libraries(${TARGET_BIN} PRIVATE SDL2)
target_link_libraries(${TARGET_BIN} PRIVATE SDL2_image)
set_property(TARGET ${TARGET_BIN} PROPERTY CXX_STANDARD 17)
target_compile_options(${TARGET_BIN} PRIVATE -g -Wall -Wextra -O0 -lSDL_image)
set_target_properties(${TARGET_BIN} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
