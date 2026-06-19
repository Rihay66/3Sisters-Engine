cmake_minimum_required(VERSION 3.28.0)
set (CMAKE_C_STANDARD 17)
project(3Sisters-Engine VERSION 1.0.0 LANGUAGES C)
cmake_policy(SET CMP0072 NEW)

# Set the install prefix to the build folder's lib directory
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}")

# Set the library output to the build folder's bin directory
set(CMAKE_LIBRARY_PREFIX "${CMAKE_BINARY_DIR}/bin")

# Grab all engine source files
file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/3Sisters-Engine/src/*
)

# Create engine as a static library and add source files
add_library(3Sisters-Engine STATIC ${SOURCE_FILES})

# Find all dependencies needed to compile

# include subdirectory
add_subdirectory(vendor)

# include all dependencies directories
target_include_directories(3Sisters-Engine PRIVATE
    vendor/sdl-src/include
)

# include all dependencies
target_link_libraries(3Sisters-Engine SDL3::SDL3-static)

# Include directory for engine's headers
set(ENGINE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Include directories for header files
target_include_directories(3Sisters-Engine PUBLIC
$<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
$<BUILD_INTERFACE:${ENGINE_INCLUDE_DIR}>
$<INSTALL_INTERFACE:include>
)

# Install the library and CMake configuration files
install(TARGETS 3Sisters-Engine EXPORT 3Sisters-EngineConfig
RUNTIME_DEPENDENCY_SET engine_depen
ARCHIVE DESTINATION ${CMAKE_LIBRARY_PREFIX}
LIBRARY DESTINATION ${CMAKE_LIBRARY_PREFIX}
RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}
INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Include all headers of the engine and dependecies (only the ones that don't get included)
install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/sdl-src/include/SDL3
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-src/include/GLFW
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.h"
)

install(
    DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm-src/glm
    DESTINATION "${CMAKE_SOURCE_DIR}/build/include"
    FILES_MATCHING PATTERN "*.hpp"
)